#include "Enemy_Armadillo.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Map.h"
#include "../Utils.cpp"

EnemyArmadillo::EnemyArmadillo() : Entity(EntityType::ENEMY_ARMADILLO)
{
	name.Create("enemyArmadillo");
}

EnemyArmadillo::~EnemyArmadillo() {}

bool EnemyArmadillo::Awake() {

	
	

	idleAnim.LoadAnimation("enemyArmadillo_idle");
	tilesView = 5;


	return true;
}

bool EnemyArmadillo::Start() {

	//initilize textures
	texturePath = parameters.attribute("texturepath").as_string();


	texture = app->tex->Load(texturePath);
	originalPosition = position;

	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 16, bodyType::DYNAMIC);
	pbody->ctype = ColliderType::ENEMY;
	pbody->listener = this;

	state = EntityState::IDLE;
	speed = 0.02f;


	return true;
}

bool EnemyArmadillo::Update(float dt)
{
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	origPos = app->map->WorldToMap(position.x + 1, position.y + 1);


	if (dist(app->scene->getPlayer()->position.x, app->scene->getPlayer()->position.y, position.x, position.y) < app->map->GetTileWidth() * tilesView) {
		onView = true;
		
		targPos = app->map->WorldToMap(app->scene->getPlayer()->position.x, app->scene->getPlayer()->position.y);

		app->map->pathfindingFloor->CreatePath(origPos, targPos);
		lastPath = *app->map->pathfindingFloor->GetLastPath();
		
	}
	else {
		onView = false;

		targPos = app->map->WorldToMap(originalPosition.x, originalPosition.y);
		app->map->pathfindingFloor->CreatePath(origPos, targPos);
		lastPath = *app->map->pathfindingFloor->GetLastPath();
	}


	
	


	b2Vec2 vel = b2Vec2(0,0);

	
	vel.y -= GRAVITY_Y;
	iPoint nextPathTile;
	lastPath.Pop(nextPathTile);
	if (nextPathTile.x < origPos.x) {
		isFacingLeft = true;
		vel.x -= speed * dt;
	}
	else {
		isFacingLeft = false;
		vel.x += speed * dt;
	}
	



	pbody->body->SetLinearVelocity(vel);


	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;




	switch (state)
	{
	case EntityState::IDLE:
		currentAnimation = &idleAnim;
		break;
	case EntityState::RUNNING:
		break;
	case EntityState::ATTACKING:
		break;
	case EntityState::JUMPING:
		break;
	case EntityState::FALLING:
		break;
	case EntityState::DYING:
		break;
	default:
		currentAnimation = &idleAnim;
		break;
	}


	currentAnimation->Update();


	return true;
}

bool EnemyArmadillo::PostUpdate() {
	
	if (currentAnimation == nullptr) { currentAnimation = &idleAnim; }
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	
	if (isFacingLeft) {
		app->render->DrawTexture(texture, position.x, position.y, SDL_FLIP_HORIZONTAL, &rect);
	}
	else {
		app->render->DrawTexture(texture, position.x, position.y, SDL_FLIP_NONE, &rect);
	}

	if (app->debug) {
		

		for (uint i = 0; i <  lastPath.Count(); ++i)
		{
			iPoint pos = app->map->MapToWorld(lastPath.At(i)->x, lastPath.At(i)->y);
			app->render->DrawTexture(app->map->pathfindingFloor->tilePathTex, pos.x, pos.y);
		}
	}


	return true;
}

bool EnemyArmadillo::CleanUp()
{
	return true;
}

void EnemyArmadillo::OnCollision(PhysBody* physA, PhysBody* physB)
{

}

void EnemyArmadillo::OnExitCollision(PhysBody* physA, PhysBody* physB)
{

}
