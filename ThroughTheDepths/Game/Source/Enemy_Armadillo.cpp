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
#include "Timer.h"
#include "Map.h"
#include "../Utils.cpp"

EnemyArmadillo::EnemyArmadillo() : Entity(EntityType::ENEMY_ARMADILLO)
{
	name.Create("enemyArmadillo");
}

EnemyArmadillo::~EnemyArmadillo() {}

bool EnemyArmadillo::Awake() {

	idleAnim.LoadAnimation(name.GetString(), "idleAnim");
	runAnim.LoadAnimation(name.GetString(), "runAnim");
	attackAnim.LoadAnimation(name.GetString(), "attackAnim");
	attackLoopAnim.LoadAnimation(name.GetString(), "attackLoopAnim");
	trackAnim.LoadAnimation(name.GetString(), "trackAnim");
	dieAnim.LoadAnimation(name.GetString(), "dieAnim");
	stopAnim.LoadAnimation(name.GetString(), "stopAttackAnim");


	state = EntityState::IDLE;
	goToPath = false;

	return true;
}

bool EnemyArmadillo::Start() {

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

	

	return true;
}

bool EnemyArmadillo::Update(float dt)
{

	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;


	





	origPos = app->map->WorldToMap(position.x+32, position.y);
	playerPos = app->scene->getPlayer()->position;
	targPos = app->map->WorldToMap(app->scene->getPlayer()->position.x, app->scene->getPlayer()->position.y);


	

	if (!cansado) {
		if (dist(playerPos, position) < app->map->GetTileWidth() * tilesView) {
			onView = true;
			state = EntityState::RUNNING;
			speed = runSpeed;


			app->map->pathfindingFloor->CreatePath(origPos, targPos);
			lastPath = *app->map->pathfindingFloor->GetLastPath();

			if (dist(playerPos, position) < app->map->GetTileWidth() * tilesAttack) {
				if (!isAttacking) {
					isAttacking = true;
					stopAnim.Reset();
					attackTimer.Start();
				}
			}


		}
		else {

			onView = false;
			speed = walkSpeed;

			if (lastPath.Count() == 0) {

				if (!idleAnim.HasFinished()) {
					state = EntityState::IDLE;
				}
				else {

					idleAnim.Reset();
					state = EntityState::RUNNING;
					targPos = iPoint(originalPosition.x + rand() % tilesView * 2 - tilesView, originalPosition.y);
					app->map->pathfindingFloor->CreatePath(origPos, targPos);
					lastPath = *app->map->pathfindingFloor->GetLastPath();

				}
			}
			else {
				state = EntityState::RUNNING;
			}
		}

		if (isAttacking) {
			speed = attackSpeed*1.5;
			state = EntityState::ATTACKING;
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
		vel.y -= GRAVITY_Y;

		if (lastPath.Count() > 0) {
			iPoint* nextPathTile;
			nextPathTile = lastPath.At(lastPath.Count() - 1);
			//LOG("LAST PATH X: %d  ENEMY X: %d", nextPathTile->x, origPos.x);
			if (nextPathTile->x < origPos.x) {
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

		}

		pbody->body->SetLinearVelocity(vel);
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

	currentAnimation->Update();

	return true;
}

bool EnemyArmadillo::PostUpdate() {

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
			app->render->DrawTexture(app->map->pathfindingFloor->tilePathTex, pos.x, pos.y);
		}
	}


	return true;
}

bool EnemyArmadillo::CleanUp()
{

	app->physics->GetWorld()->DestroyBody(pbody->body);
	SDL_DestroyTexture(texture);


	return true;
}

void EnemyArmadillo::OnCollision(PhysBody* physA, PhysBody* physB)
{
	if (physB->ctype == ColliderType::PLAYER_PROYECTILE) {
		isDying = true;
	}
}

void EnemyArmadillo::OnExitCollision(PhysBody* physA, PhysBody* physB)
{

}
