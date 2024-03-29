#include "Enemy_Boss.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Enemy_BossFireball.h"
#include "Enemy_BossAttack.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Timer.h"
#include "Map.h"
#include "Interface.h"
#include "SceneMainMenu.h"
#include "FadeToBlack.h"
#include "../Utils.cpp"
#include <ctime>
#include <random>

EnemyBoss::EnemyBoss() : Entity(EntityType::BOSS)
{
	name.Create("enemyBoss");
}

EnemyBoss::~EnemyBoss() {}

bool EnemyBoss::Awake() {

	idleAnim.LoadAnimation(name.GetString(), "idleAnim");
	walkAnim.LoadAnimation(name.GetString(), "walkAnim");
	runAnim.LoadAnimation(name.GetString(), "runAnim");

	attackAnim.LoadAnimation(name.GetString(), "attackAnim");
	attackShootStartAnim.LoadAnimation(name.GetString(), "attackShootStart");
	attackShootAnim.LoadAnimation(name.GetString(), "attackShootAnim");
	attackShootEndAnim.LoadAnimation(name.GetString(), "attackShootEnd");
	attackJumpAnim.LoadAnimation(name.GetString(), "jumpAttackAnim");

	dmgAnim.LoadAnimation(name.GetString(), "dmgAnim");
	dieAnim.LoadAnimation(name.GetString(), "dieAnim");

	teleportInAnim.LoadAnimation(name.GetString(), "dieAnim");
	teleportOutAnim.LoadAnimation(name.GetString(), "dieAnimInvert");



	state = EntityState::IDLE;
	goToPath = false;

	return true;
}

bool EnemyBoss::Start() {

	//initilize textures
	texturePath = parameters.attribute("texturepath").as_string();
	texturePathFireball = parameters.attribute("fireballtexturepath").as_string();

	walkSpeed = parameters.attribute("walkSpeed").as_float();
	runSpeed = parameters.attribute("runSpeed").as_float();
	attackSpeed = parameters.attribute("attackSpeed").as_float();
	tilesView = parameters.attribute("tilesView").as_int();
	tilesAttack = parameters.attribute("tilesAttack").as_int();
	attackTimeMax = parameters.attribute("attackTimerMax").as_int();
	speed = walkSpeed;
	speedFireball = parameters.attribute("fireballSpeed").as_float();

	texture = app->tex->Load(texturePath);
	originalPosition = app->map->WorldToMap(position.x, position.y);

	//pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 50, bodyType::DYNAMIC);
	pbody = app->physics->CreateRectangle(position.x + 16, position.y + 16, 80,80, bodyType::DYNAMIC);
	pbody->body->SetFixedRotation(true);
	pbody->ctype = ColliderType::ENEMY;
	pbody->listener = this;

	//Atravesar enemigos
	b2Filter enemyFilter;
	enemyFilter.categoryBits = static_cast<uint16_t>(ColliderType::PLATFORM);
	enemyFilter.maskBits = 0xFFFF & ~static_cast<uint16_t>(ColliderType::PLATFORM);
	pbody->body->GetFixtureList()->SetFilterData(enemyFilter);



	ataqueBoss = app->audio->LoadAudioFX("audioAtaqueArmadillo");
	muerteBoss = app->audio->LoadAudioFX("audioMuerteArmadillo");
	caminarBoss = app->audio->LoadAudioFX("audioCaminarArmadillo");

	activeBoss = false;

	return true;
}

bool EnemyBoss::Update(float dt)
{


	if (!active) {

		pbody->body->SetActive(false);
		return true;
	}

	//Setting position for movement
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;
	origPos = app->map->WorldToMap(position.x + 32, position.y);
	playerPos = app->scene->getPlayer()->position;
	targPos = app->map->WorldToMap(app->scene->getPlayer()->position.x, app->scene->getPlayer()->position.y);
	targPos.x += 1;
	


	Movement(dt);

	Dying();
	
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	AnimationState();

	currentAnimation->Update();

	return true;
}

bool EnemyBoss::PostUpdate() {

	if (currentAnimation == nullptr) { currentAnimation = &idleAnim; }
	SDL_Rect rect = currentAnimation->GetCurrentFrame();

	if (!isFacingLeft) {
		app->render->DrawTexture(texture, position.x - 110, position.y - 86, SDL_FLIP_HORIZONTAL, &rect);
	}
	else {
		app->render->DrawTexture(texture, position.x - 110, position.y - 86, SDL_FLIP_NONE, &rect);
	}

	return true;
}

bool EnemyBoss::CleanUp()
{

	app->physics->GetWorld()->DestroyBody(pbody->body);
	SDL_DestroyTexture(texture);


	return true;
}

void EnemyBoss::OnCollision(PhysBody* physA, PhysBody* physB)
{
	if (physB->ctype == ColliderType::PLAYER_PROYECTILE) {
		health--;
		if (!isDying && health<=0) {
			app->audio->StopFx(id);
			app->audio->PlayFx(muerteBoss, id);
			isDying = true;
		}
		
	}
}

void EnemyBoss::OnExitCollision(PhysBody* physA, PhysBody* physB)
{

}

void EnemyBoss::Movement(float dt)
{
	if (activeBoss) {
		

		


		LOG("Boss x: %d", METERS_TO_PIXELS(pbody->body->GetTransform().p.x));
		b2Vec2 vel = b2Vec2(0, pbody->body->GetLinearVelocity().y);

		if (health > (maxHealth / 3) * 2 || volverAAndar) {
			if (state != EntityState::ATTACKING && state != EntityState::SECONDARY_ATTACK) {
				float actualSpeed;

				if (toRunTimer.ReadSec() >= 2 || health <= (maxHealth / 3) * 2) {
					actualSpeed = runSpeed;
					state = EntityState::RUNNING;
				}
				else {
					actualSpeed = walkSpeed;
					state = EntityState::WALKAROUND;
				}


				if (playerPos.x < position.x) {
					vel.x -= actualSpeed * dt;
					isFacingLeft = true;
				}
				else {
					vel.x += actualSpeed * dt;
					isFacingLeft = false;
				}

				if (health <= (maxHealth / 3) * 2) {
					//eNTRA EN SEGUNDA FASE, SE HACE TP A LA DERECHA O ZIQUIERDA, Y DISPARA LOS PROYECTILES
					if (doAttackShootTimer.ReadSec() > 1) {
						//state = EntityState::SECONDARY_ATTACK;
					}
				}
				else if (health <= maxHealth / 3) {
					//Entra en la 3ra fase, hay que esquivar los ataques del salto

				}

				if (abs(playerPos.x - position.x) <= 96) {
					state = EntityState::ATTACKING;
					if (!haSpawnAtaque){
						EnemyBossAttack* attack = (EnemyBossAttack*)app->entityManager->CreateEntity(EntityType::BOSS_ATTACK);
						attack->texturePath = texturePathFireball;
						if (isFacingLeft) {
							attack->position = iPoint(position.x - 80, position.y + 30);
						}
						else {
							attack->position = iPoint(position.x + 80, position.y + 30);
						}
						

						
						attack->Start();
					}
				}


			}
			else {
				if (state == EntityState::ATTACKING) {
					if (attackAnim.HasFinished()) {
						state = EntityState::WALKAROUND;
						toRunTimer.Start();
						doAttackShootTimer.Start();
						attackAnim.Reset();
					}
				}
				else if (state == EntityState::SECONDARY_ATTACK) {
					if (attackShootAnim.HasFinished()) {
						state = EntityState::WALKAROUND;
						attackShootAnim.Reset();
						doAttackShootTimer.Start();
					}
				}

			}
		}
		else if (health > 0 && !volverAAndar) {
			//Segunda fase
			if (faseTimer.ReadMSec() > faseTimerTime) {
				state = EntityState::TPIN;
				if (teleportInAnim.HasFinished()) {
					//Cambiar posicion
					if (!setPosicionTpRandom) {
						//Random 1

						std::random_device rd;
						std::mt19937 generator(rd());
						std::uniform_int_distribution<int> distribution(1, 2);
						int random_number = distribution(generator);

						if (random_number == 1) {

							pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(3385), pbody->body->GetTransform().p.y), 0);
							isFacingLeft = false;
						}
						else {
							pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(3787), pbody->body->GetTransform().p.y), 0);
							isFacingLeft = true;
						}


						setPosicionTpRandom = true;
					}

					state = EntityState::TPOUT;

					if (teleportOutAnim.HasFinished()) {
						state = EntityState::SECONDARY_ATTACK;




						if (attackShootDurationTimer.ReadSec() > attackShootDuration) {
							state = EntityState::SECONDARY_ATTACK_STOP;

							if (attackShootEndAnim.HasFinished()) {
								//Andar o lo que sea
								LOG("AAAAAAAAAAAAAAAAAAAAAAAAAAAA");

								volverAAndar = true;
								attackAnim.Reset();
								state = EntityState::RUNNING;
							}
						}
						else {
							if (testTimer.ReadMSec() > 600) {
								testTimer.Start();
								EnemyBossFireball* fireball = (EnemyBossFireball*)app->entityManager->CreateEntity(EntityType::BOSS_FIREBALL);
								fireball->texturePath = texturePathFireball;
								fireball->position = iPoint(position.x, position.y + 20);

								fireball->speed = (isFacingLeft) ? -0.20f : 0.20f;
								fireball->isFacingLeft = isFacingLeft;
								fireball->Start();
							}
						}
					}
					else {
						attackShootDurationTimer.Start();
						testTimer.Start();
					}

				}
			}
			
		}
		else {
			//Tercera fase
		}


		pbody->body->SetLinearVelocity(vel);



		


	}
	else {
		//Boss innactivo
		toRunTimer.Start();
	}
	
}
void EnemyBoss::Dying()
{
	if (isDying) {

		pbody->body->SetActive(false);
		b2Vec2 vel = b2Vec2(0, 0);
		vel.y -= GRAVITY_Y;
		pbody->body->SetLinearVelocity(vel);
		state = EntityState::DYING;

		
		app->interface->showWinScreen = true;
		if (!goToMainMenuAfterWin.started) {
			goToMainMenuAfterWin.Start();
		}
		else {
			if (goToMainMenuAfterWin.ReadSec() >= 3) {
				app->fadeToBlack->FadeToBlackTransition(app->scene, app->sceneMainMenu);
			}
		}
		

		if (dieAnim.HasFinished() && goToMainMenuAfterWin.ReadSec() >= 3) {
			active = false;

		}

	}
}

void EnemyBoss::AnimationState()
{
	switch (state)
	{
	case EntityState::IDLE:				currentAnimation = &idleAnim; break;
	case EntityState::RUNNING:			currentAnimation = &runAnim; break;
	case EntityState::WALKAROUND:		currentAnimation = &walkAnim; break;
	case EntityState::ATTACKING:		currentAnimation = &attackAnim;break;
	case EntityState::SECONDARY_ATTACK:
		currentAnimation = &attackShootStartAnim;    
		if (attackShootStartAnim.HasFinished()) {
			currentAnimation = &attackShootAnim;
		}
		break;
	case EntityState::SECONDARY_ATTACK_STOP: currentAnimation = &attackShootEndAnim; break;
	case EntityState::JUMPING:			currentAnimation = &attackJumpAnim; break;
	case EntityState::DMG:				currentAnimation = &dmgAnim; break;
	case EntityState::DYING:			currentAnimation = &dieAnim; break;

	case EntityState::TPIN:				currentAnimation = &teleportInAnim; break;
	case EntityState::TPOUT:			currentAnimation = &teleportOutAnim; break;

	default:							currentAnimation = &idleAnim; break;
	}

}
