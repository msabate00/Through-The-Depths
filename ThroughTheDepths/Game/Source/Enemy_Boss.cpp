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
	dmgAnim.LoadAnimation(name.GetString(), "dmgAnim");
	dieAnim.LoadAnimation(name.GetString(), "dieAnim");



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
		app->render->DrawTexture(texture, position.x - 110, position.y - 75, SDL_FLIP_HORIZONTAL, &rect);
	}
	else {
		app->render->DrawTexture(texture, position.x - 110, position.y - 75, SDL_FLIP_NONE, &rect);
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
	if (activeBoss) {

		b2Vec2 vel = b2Vec2(0, pbody->body->GetLinearVelocity().y);
		if (playerPos.x < position.x) {
			vel.x -= walkSpeed * dt;
			isFacingLeft = true;
		}
		else {
			vel.x += walkSpeed * dt;
			isFacingLeft = false;
		}


		pbody->body->SetLinearVelocity(vel);

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
	case EntityState::RUNNING:			currentAnimation = &runAnim; break;
	case EntityState::ATTACKING:		currentAnimation = &attackAnim;break;
	case EntityState::SECONDARY_ATTACK:	currentAnimation = &attack2Anim;break;
	case EntityState::DMG:				currentAnimation = &dmgAnim; break;
	case EntityState::DYING:			currentAnimation = &dieAnim; break;

	default:							currentAnimation = &idleAnim; break;
	}

}
