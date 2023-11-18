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

	return true;
}

bool EnemyArmadillo::Update(float dt)
{
	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;



	if (dist(app->scene->getPlayer()->position.x, app->scene->getPlayer()->position.y, position.x, position.y) < app->map->GetTileWidth() * tilesView) {
		onView = true;
		iPoint origPos = app->map->WorldToMap(position.x+1, position.y+1);
		iPoint targPos = app->map->WorldToMap(app->scene->getPlayer()->position.x, app->scene->getPlayer()->position.y);

		app->map->pathfinding->CreatePath(origPos, targPos);

	}
	else {
		onView = false;
	}

	
	





	currentAnimation = &idleAnim;


	currentAnimation->Update();


	return true;
}

bool EnemyArmadillo::PostUpdate() {
	

	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	
	if (isFacingLeft) {
		app->render->DrawTexture(texture, position.x, position.y, SDL_FLIP_HORIZONTAL, &rect);
	}
	else {
		app->render->DrawTexture(texture, position.x, position.y, SDL_FLIP_NONE, &rect);
	}

	if (onView && app->debug) {
		const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath();

		for (uint i = 0; i < path->Count(); ++i)
		{
			iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
			app->render->DrawTexture(app->map->pathfinding->tilePathTex, pos.x, pos.y);
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
