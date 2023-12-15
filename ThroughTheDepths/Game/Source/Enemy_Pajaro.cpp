#include "Enemy_Pajaro.h"
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

EnemyPajaro::EnemyPajaro() : Entity(EntityType::ENEMY_ARMADILLO)
{
	name.Create("enemyPaloma");
}

EnemyPajaro::~EnemyPajaro() {}

bool EnemyPajaro::Awake() {

	idleAnim.LoadAnimation(name.GetString(), "idleAnim");
	runAnim.LoadAnimation(name.GetString(), "idleAnim");
	attackAnim.LoadAnimation(name.GetString(), "attackAnim");
	dieAnim.LoadAnimation(name.GetString(), "dieAnim");



	state = EntityState::IDLE;
	goToPath = false;

	return true;
}

bool EnemyPajaro::Start() {

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

	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 10, bodyType::DYNAMIC);
	pbody->ctype = ColliderType::ENEMY;
	pbody->listener = this;
	pbody->body->SetGravityScale(0);


	return true;
}

bool EnemyPajaro::Update(float dt)
{

	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;


	origPos = app->map->WorldToMap(position.x + 32, position.y);
	playerPos = app->scene->getPlayer()->position;
	targPos = app->map->WorldToMap(app->scene->getPlayer()->position.x, app->scene->getPlayer()->position.y);

	if (!isDying) {



		if (dist(playerPos, position) < app->map->GetTileWidth() * tilesView) {
			onView = true;
			state = EntityState::RUNNING;
			speed = runSpeed;
			if (!isAttacking) {
				targPos.y -= 3;
			}
			app->map->pathfindingFly->CreatePath(origPos, targPos);
			lastPath = *app->map->pathfindingFly->GetLastPath();

			if (origPos == targPos) {
				if (!isAttacking) {
					isAttacking = true;
					attackTimer.Start();
				}
			}

		}

		if (!cansado) {

		}




		b2Vec2 vel = b2Vec2(0, 0);

		if (!isAttacking) {
			if (lastPath.Count() > 0) {
				iPoint* nextPathTile;
				nextPathTile = lastPath.At(lastPath.Count() - 1);


				//Movimiento en x
				if (nextPathTile->x < origPos.x) {
					isFacingLeft = true;
					vel.x -= speed * dt;
				}
				else {
					isFacingLeft = false;
					vel.x += speed * dt;
				}

				//Movimiento en y
				if (nextPathTile->y < origPos.y) {

					vel.y -= speed * dt;
				}
				else {
					vel.y += speed * dt;
				}

				if (nextPathTile->x == origPos.x && nextPathTile->y == origPos.y) {
					lastPath.Pop(*nextPathTile);
				}

			}
		}
		else {
			state = EntityState::ATTACKING;
			vel.y = -GRAVITY_Y * 2;
			if (lastPath.Count() > 0) {
				iPoint* nextPathTile;
				nextPathTile = lastPath.At(lastPath.Count() - 1);


				//Movimiento en x
				if (nextPathTile->x < origPos.x) {
					//isFacingLeft = true;
					vel.x -= speed * dt;
				}
				else {
					isFacingLeft = false;
					vel.x += speed * dt;
				}

				//Movimiento en y
				if (nextPathTile->y < origPos.y) {

					vel.y -= speed * dt;
				}
				else {
					vel.y += speed * dt;
				}

				if (nextPathTile->x == origPos.x && nextPathTile->y == origPos.y) {
					lastPath.Pop(*nextPathTile);
				}
				if (lastPosY == position.y) {
					isAttacking = false;
				}

			}
		}

		pbody->body->SetLinearVelocity(vel);
	}

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

	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;
	lastPosY = position.y;
	switch (state)
	{
		case EntityState::IDLE:				currentAnimation = &runAnim; break;
			//case EntityState::STOP_ATTACKING:	currentAnimation = &stopAnim; if (stopAnim.HasFinished()) { currentAnimation = &idleAnim; } break;
		case EntityState::RUNNING:			currentAnimation = &runAnim; break;
		case EntityState::ATTACKING:		currentAnimation = &attackAnim;break;
		case EntityState::JUMPING:			currentAnimation = &runAnim; break;
		case EntityState::FALLING:			currentAnimation = &runAnim; break;
		case EntityState::DYING:			currentAnimation = &dieAnim; break;
			//case EntityState::TRACK:			currentAnimation = &trackAnim; break;
		default:							currentAnimation = &runAnim; break;
	}

	currentAnimation->Update();

	return true;
}

bool EnemyPajaro::PostUpdate() {

	if (currentAnimation == nullptr) { currentAnimation = &idleAnim; }
	SDL_Rect rect = currentAnimation->GetCurrentFrame();

	if (isFacingLeft) {
		app->render->DrawTexture(texture, position.x - 4, position.y - 14, SDL_FLIP_HORIZONTAL, &rect);
	}
	else {
		app->render->DrawTexture(texture, position.x - 4, position.y - 14, SDL_FLIP_NONE, &rect);
	}

	if (app->debug) {


		for (uint i = 0; i < lastPath.Count(); ++i)
		{
			iPoint pos = app->map->MapToWorld(lastPath.At(i)->x, lastPath.At(i)->y);
			app->render->DrawTexture(app->map->tilePathTexBrown, pos.x, pos.y);
		}
	}


	return true;
}

bool EnemyPajaro::CleanUp()
{

	app->physics->GetWorld()->DestroyBody(pbody->body);
	SDL_DestroyTexture(texture);


	return true;
}

void EnemyPajaro::OnCollision(PhysBody* physA, PhysBody* physB)
{
	if (physB->ctype == ColliderType::PLAYER_PROYECTILE) {
		isDying = true;
	}
}

void EnemyPajaro::OnExitCollision(PhysBody* physA, PhysBody* physB)
{

}
