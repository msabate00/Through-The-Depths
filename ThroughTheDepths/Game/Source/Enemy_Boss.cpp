#include "Enemy_Boss.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Timer.h"
#include "Map.h"
#include "../Utils.cpp"

EnemyBoss::EnemyBoss() : Entity(EntityType::BOSS)
{
	name.Create("enemyBoss");
}

EnemyBoss::~EnemyBoss() {}

bool EnemyBoss::Awake() {

	idleAnim.LoadAnimation(name.GetString(), "idleAnim");
	runAnim.LoadAnimation(name.GetString(), "walkAnim");
	attackAnim.LoadAnimation(name.GetString(), "attackAnim");
	attackAnim.LoadAnimation(name.GetString(), "dmgAnim");
	/*attackLoopAnim.LoadAnimation(name.GetString(), "attackLoopAnim");
	trackAnim.LoadAnimation(name.GetString(), "trackAnim");*/
	dieAnim.LoadAnimation(name.GetString(), "dieAnim");
	/*stopAnim.LoadAnimation(name.GetString(), "stopAttackAnim");*/


	state = EntityState::IDLE;
	goToPath = false;

	return true;
}

bool EnemyBoss::Start() {

	//initilize textures
	texturePath = parameters.attribute("texturepath").as_string();

	walkSpeed = parameters.attribute("walkSpeed").as_float();
	runSpeed = parameters.attribute("runSpeed").as_float();
	attackSpeed = parameters.attribute("attackSpeed").as_float();
	tilesView = parameters.attribute("tilesView").as_int();
	tilesAttack = parameters.attribute("tilesAttack").as_int();
	attackTimeMax = parameters.attribute("attackTimerMax").as_int();
	speed = walkSpeed;


	texture = app->tex->Load(texturePath);
	originalPosition = app->map->WorldToMap(position.x, position.y);

	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 50, bodyType::DYNAMIC);
	pbody->ctype = ColliderType::ENEMY;
	pbody->listener = this;

	//Atravesar enemigos
	b2Filter enemyFilter;
	enemyFilter.categoryBits = static_cast<uint16_t>(ColliderType::PLATFORM);
	enemyFilter.maskBits = 0xFFFF & ~static_cast<uint16_t>(ColliderType::PLATFORM);
	pbody->body->GetFixtureList()->SetFilterData(enemyFilter);


	pathfinding = new PathFinding();
	uchar* navigationMap = NULL;

	app->map->CreateNavigationMap(app->map->mapData.width, app->map->mapData.height, &navigationMap, app->map->navigationLayer_Floor);
	pathfinding->SetNavigationMap((uint)app->map->mapData.width, (uint)app->map->mapData.height, navigationMap);

	RELEASE_ARRAY(navigationMap);

	ataqueBoss = app->audio->LoadAudioFX("audioAtaqueArmadillo");
	muerteBoss = app->audio->LoadAudioFX("audioMuerteArmadillo");
	caminarBoss = app->audio->LoadAudioFX("audioCaminarArmadillo");

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

	if (isFacingLeft) {
		app->render->DrawTexture(texture, position.x - 4, position.y - 64, SDL_FLIP_HORIZONTAL, &rect);
	}
	else {
		app->render->DrawTexture(texture, position.x - 4, position.y - 64, SDL_FLIP_NONE, &rect);
	}

	if (app->debug) {


		for (uint i = 0; i < lastPath.Count(); ++i)
		{
			iPoint pos = app->map->MapToWorld(lastPath.At(i)->x, lastPath.At(i)->y);
			if (isAttacking) {
				app->render->DrawTexture(app->map->tilePathTexRed, pos.x, pos.y, 1.0f);
			}
			else {
				app->render->DrawTexture(app->map->tilePathTexBrown, pos.x, pos.y, 1.0f);
			}
		}
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
		if (!isDying) {
			app->audio->StopFx(id);
			app->audio->PlayFx(muerteBoss, id);
		}
		isDying = true;
	}
}

void EnemyBoss::OnExitCollision(PhysBody* physA, PhysBody* physB)
{

}

void EnemyBoss::Movement(float dt)
{
	if (!cansado) {
		if (dist(playerPos, position) < app->map->GetTileWidth() * tilesView) {
			onView = true;
			state = EntityState::RUNNING;
			speed = runSpeed;

			pathfinding->CreatePath(origPos, targPos);
			lastPath = *pathfinding->GetLastPath();

			if (dist(playerPos, position) < app->map->GetTileWidth() * tilesAttack) {
				if (!isAttacking) {
					isAttacking = true;
					stopAnim.Reset();
					attackTimer.Start();
				}
			}
		}
		else {
			//lastPath.Clear();
			if (onView) { lastPath.Clear(); }
			onView = false;
			speed = walkSpeed;

			if (lastPath.Count() == 0) {
				if (!idleAnim.HasFinished()) {
					state = EntityState::IDLE;
				}
				else {

					state = EntityState::RUNNING;

					b2Vec2 vel = b2Vec2(0, 0);
					vel.y -= GRAVITY_Y;
					if (isFacingLeft) {
						origPos.x -= 1;
						if (pathfinding->IsWalkable(origPos)) {
							vel.x -= speed * dt;
						}
						else {
							isFacingLeft = false;
						}
					}
					else {
						origPos.x += 1;
						if (pathfinding->IsWalkable(origPos)) {
							vel.x += speed * dt;
						}
						else {
							isFacingLeft = true;
						}
					}

					pbody->body->SetLinearVelocity(vel);
				}
			}
		}

		if (isAttacking) {
			speed = attackSpeed * 1.5;
			state = EntityState::ATTACKING;
			app->audio->PlayFx(ataqueBoss, id);
			if (attackTimer.ReadMSec() > attackTimeMax * 1000) {
				cansado = true;
				cansadoTimer.Start();
				isAttacking = false;

				attackAnim.Reset();
				attackLoopAnim.Reset();

			}
		}
		else {
			if (attackTimer.ReadMSec() < attackTimeMax + 2 * 1000) {
				//Descansar

				state = EntityState::IDLE;
				speed = 0;
			}
		}


		b2Vec2 vel = b2Vec2(0, 0);
		vel.y -= GRAVITY_Y * 10;

		if (lastPath.Count() > 0) {
			iPoint* nextPathTile;
			nextPathTile = lastPath.At(lastPath.Count() - 1);
			//LOG("LAST PATH X: %d  ENEMY X: %d", nextPathTile->x, origPos.x);

			if (!isAttacking)
			{
				app->audio->PlayFx(caminarBoss, id);
			}


			if (nextPathTile->x == origPos.x) {
				//cansado = true;
				//cansadoTimer.Start();
				//isFacingLeft = false;

			}
			else if (nextPathTile->x < origPos.x) {
				isFacingLeft = true;
				vel.x -= speed * dt;
			}
			else {
				isFacingLeft = false;
				vel.x += speed * dt;
			}

			if (nextPathTile->x == origPos.x) {
				lastPath.Pop(*nextPathTile);
			}
			pbody->body->SetLinearVelocity(vel);
		}


	}
	else {

		if (cansadoTimer.ReadMSec() < 2000) {
			b2Vec2 vel = b2Vec2(0, 0);
			vel.y -= GRAVITY_Y;
			pbody->body->SetLinearVelocity(vel);
			state = EntityState::STOP_ATTACKING;


			if (idleAnim.HasFinished()) {
				idleAnim.Reset();
			}

		}
		else {
			idleAnim.Reset();
			cansado = false;
		}

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

		if (dieAnim.HasFinished()) {
			active = false;

		}

	}
}

void EnemyBoss::AnimationState()
{
	switch (state)
	{
	case EntityState::IDLE:				currentAnimation = &idleAnim; break;
	case EntityState::STOP_ATTACKING:	currentAnimation = &stopAnim; if (stopAnim.HasFinished()) { currentAnimation = &idleAnim; } break;
	case EntityState::RUNNING:			currentAnimation = &runAnim; break;
	case EntityState::ATTACKING:		currentAnimation = &attackAnim; if (attackAnim.HasFinished()) { currentAnimation = &attackLoopAnim; }   break;
	case EntityState::JUMPING:			currentAnimation = &idleAnim; break;
	case EntityState::FALLING:			currentAnimation = &idleAnim; break;
	case EntityState::DYING:			currentAnimation = &dieAnim; break;
	case EntityState::TRACK:			currentAnimation = &trackAnim; break;
	default:							currentAnimation = &idleAnim; break;
	}

}
