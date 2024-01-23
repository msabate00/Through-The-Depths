#include "Enemy_BossFireball.h"
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
#include <ctime>
#include <random>

EnemyBossFireball::EnemyBossFireball() : Entity(EntityType::BOSS)
{
	name.Create("enemyBossFireball");
}

EnemyBossFireball::~EnemyBossFireball() {}

bool EnemyBossFireball::Awake() {

	idleAnim.LoadAnimation(name.GetString(), "idleAnim");
	



	state = EntityState::IDLE;
	goToPath = false;

	return true;
}

bool EnemyBossFireball::Start() {

	//initilize textures
	//texturePath = parameters.attribute("texturepath").as_string();

	texture = app->tex->Load(texturePath);
	originalPosition = app->map->WorldToMap(position.x, position.y);

	//pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 50, bodyType::DYNAMIC);
	pbody = app->physics->CreateRectangleSensor(position.x + 16, position.y + 16, 10,10, bodyType::DYNAMIC);
	pbody->body->SetFixedRotation(true);
	pbody->ctype = ColliderType::FIREBALL_BOSS;
	pbody->listener = this;

	//Atravesar enemigos
	b2Filter enemyFilter;
	enemyFilter.categoryBits = static_cast<uint16_t>(ColliderType::PLATFORM);
	enemyFilter.maskBits = 0xFFFF & ~static_cast<uint16_t>(ColliderType::PLATFORM);
	pbody->body->GetFixtureList()->SetFilterData(enemyFilter);


	activeBoss = false;

	

	return true;
}

bool EnemyBossFireball::Update(float dt)
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

bool EnemyBossFireball::PostUpdate() {

	if (currentAnimation == nullptr) { currentAnimation = &idleAnim; }
	SDL_Rect rect = currentAnimation->GetCurrentFrame();

	if (isFacingLeft) {
		app->render->DrawTexture(texture, position.x + 7, position.y, SDL_FLIP_HORIZONTAL, &rect);
	}
	else {
		app->render->DrawTexture(texture, position.x-7 , position.y, SDL_FLIP_NONE, &rect);
	}

	return true;
}

bool EnemyBossFireball::CleanUp()
{

	app->physics->GetWorld()->DestroyBody(pbody->body);
	SDL_DestroyTexture(texture);


	return true;
}

void EnemyBossFireball::OnCollision(PhysBody* physA, PhysBody* physB)
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

void EnemyBossFireball::OnExitCollision(PhysBody* physA, PhysBody* physB)
{

}

void EnemyBossFireball::Movement(float dt)
{
	b2Vec2 vel = b2Vec2(0, 0);
	vel.x = speed * dt;
	pbody->body->SetLinearVelocity(vel);
	
}
void EnemyBossFireball::Dying()
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

void EnemyBossFireball::AnimationState()
{
	currentAnimation = &idleAnim;
	/*switch (state)
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
	}*/

}
