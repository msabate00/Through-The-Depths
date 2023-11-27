#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Window.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Particles.h"
#include "FadeToBlack.h"
#include "Chest.h"

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("player");



	runAnim.LoadAnimation(name.GetString(), "runAnim");
	idleAnim.LoadAnimation(name.GetString(), "idleAnim");
	walkAnim.LoadAnimation(name.GetString(), "walkAnim");
	jumpUpAnim.LoadAnimation(name.GetString(), "jumpUpAnim");
	jumpDownAnim.LoadAnimation(name.GetString(), "jumpDownAnim");
	ghostAnim.LoadAnimation(name.GetString(), "dmgAnim");
	dieAnim.LoadAnimation(name.GetString(), "dieAnim");
	//attackAnim.LoadAnimation("attackAnim");
	attackAnim.LoadAnimation(name.GetString(), "attack2Anim");

}

Player::~Player() {

}

bool Player::Awake() {

	return true;
}

bool Player::Start() {


	texturePath = parameters.attribute("texturepath").as_string();
	speed = parameters.attribute("speed").as_float();
	jumpForce = parameters.attribute("jumpForce").as_float();
	invulnerableTime = parameters.attribute("invulnerableTime").as_float();

	//initilize textures
	texture = app->tex->Load(texturePath);

	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 15, bodyType::DYNAMIC);


	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;
	pbody->body->SetFixedRotation(true);
	startTransform = pbody->body->GetTransform();

	pbodyFoot = app->physics->CreateRectangle(position.x + 16, position.y + 16, 16, 3, bodyType::DYNAMIC);

	pbodyFoot->listener = this;
	pbodyFoot->ctype = ColliderType::PLAYER_FOOT;
	pbodyFoot->body->SetFixedRotation(true);


	state = EntityState::IDLE;


	pickCoinFxId = app->audio->LoadFx(parameters.child("coinAudio").attribute("path").as_string());

	uint windowH;
	uint windowW;
	app->win->GetWindowSize(windowW, windowH);

	app->render->camera.x = (-position.x * app->win->GetScale() + (windowW / 2));
	app->render->camera.y = (-position.y * app->win->GetScale() + (windowH / 2));

	return true;
}

bool Player::Update(float dt)
{

	if (app->input->GetKey(SDL_SCANCODE_F1)) {
		app->sceneLevel = 0;
		app->fadeToBlack->FadeToBlackTransition(app->scene, app->scene);
	}

	if (app->input->GetKey(SDL_SCANCODE_F2)) {
		app->sceneLevel = 1;
		app->fadeToBlack->FadeToBlackTransition(app->scene, app->scene);
	}

	if (app->input->GetKey(SDL_SCANCODE_F3)) {
		app->fadeToBlack->FadeToBlackTransition(app->scene, app->scene);
	}

	state = EntityState::IDLE;


	//Movimiento colision pies
	b2Vec2 positionFoot;
	positionFoot.x = PIXEL_TO_METERS(position.x + 0.3);
	positionFoot.y = PIXEL_TO_METERS(position.y + 0.67);

	pbodyFoot->body->SetTransform(positionFoot, pbodyFoot->body->GetAngle());

	//Sistema de atravesar plataformas
	if (traspassingColision && traspassingTimer.ReadMSec() > 100) {
		while (colisionTraspassing.Count() != 0) {
			colisionTraspassing[0]->body->SetActive(true);
			colisionTraspassing.Del(colisionTraspassing.At(0));
		}
		traspassingColision = false;
	}

	//Movimientos personaje
	if (!app->godMode && !isDying) {
		Movement(dt);

	}
	else {
		if (!isDying) {
			GodModeMovement(dt);
		}
	}

	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	//Movimiento de la camara
	if (!app->debug) {
		CameraMovement(dt);
	}

	//Sistema de Ataque
	if (isAttacking) {
		state = EntityState::ATTACKING;
		if (attackAnim.HasFinished()) {

			isAttacking = false;
			attackAnim.Reset();
			currentAnimation = &idleAnim;
		}
	}

	//Sistema de Muerte
	if (app->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN) {
		isDying = !isDying;
	}
	if (isDying) {
		state = EntityState::DYING;
		if (dieAnim.HasFinished()) {
			pbody->body->SetTransform(b2Vec2(startTransform.p.x, startTransform.p.y), startTransform.q.GetAngle());
			isDying = false;
			dieAnim.Reset();
		}
	}

	//Gestor de animaciones por estado
	switch (state)
	{
	case EntityState::IDLE:
		currentAnimation = &idleAnim;
		break;
	case EntityState::RUNNING:
		currentAnimation = &runAnim;
		break;
	case EntityState::ATTACKING:
		currentAnimation = &attackAnim;
		break;
	case EntityState::JUMPING:
		currentAnimation = &jumpUpAnim;
		break;
	case EntityState::FALLING:
		currentAnimation = &jumpDownAnim;
		break;
	case EntityState::DYING:
		currentAnimation = &dieAnim;
		break;
	default:
		currentAnimation = &idleAnim;
		break;
	}


	currentAnimation->Update();

	return true;
}

bool Player::PostUpdate() {


	if (currentAnimation == nullptr) { currentAnimation = &idleAnim; }
	SDL_Rect rect = currentAnimation->GetCurrentFrame();


	if (invulnerableTimer.ReadMSec() > invulnerableTime * 1000 || (invulnerableTimer.ReadMSec() < invulnerableTime * 1000 && app->GetFrameCount() % 2 == 0)) {
		if (isFacingLeft) {
			app->render->DrawTexture(texture, position.x - 15, position.y - 15, SDL_FLIP_HORIZONTAL, &rect);
		}
		else {
			app->render->DrawTexture(texture, position.x, position.y - 15, SDL_FLIP_NONE, &rect);
		}
	}

	return true;
}

bool Player::CleanUp()
{


	app->physics->GetWorld()->DestroyBody(pbody->body);
	app->physics->GetWorld()->DestroyBody(pbodyFoot->body);
	SDL_DestroyTexture(texture);


	return true;
}

void Player::Movement(float dt)
{
	b2Vec2 vel;
	pbody->body->GetFixtureList()[0].SetSensor(false);
	vel = b2Vec2(0, pbody->body->GetLinearVelocity().y);

	if (!isAttacking) {
		//Moverse a la izquierda
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			vel = b2Vec2(-speed * dt, pbody->body->GetLinearVelocity().y);
			isFacingLeft = true;
			state = EntityState::RUNNING;
		}

		//Moverse a la derecha
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
			vel = b2Vec2(speed * dt, pbody->body->GetLinearVelocity().y);
			isFacingLeft = false;
			state = EntityState::RUNNING;
		}

		//Atraves plataformas traspasables
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT) {

			traspassingColision = true;
			traspassingTimer.Start();

			for (int i = 0; i < colisionTraspassing.Count(); i++) {
				colisionTraspassing[i]->body->SetActive(false);
			}

		}

		vel.y -= GRAVITY_Y;
		pbody->body->SetLinearVelocity(vel);

		//Sistema de salto
		pbodyFoot->body->SetActive((abs(pbody->body->GetLinearVelocity().y) == -GRAVITY_Y));


		if (!canJump) {
			canJump = (numFootContacts != 0);

		}
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && app->input->GetKey(SDL_SCANCODE_S) != KEY_REPEAT && canJump) {
			vel.y = 0;
			pbody->body->SetLinearVelocity(vel);
			pbody->body->ApplyLinearImpulse(b2Vec2(0, GRAVITY_Y * jumpForce), pbody->body->GetWorldCenter(), true);
			canJump = false;
		}


	}
	if (vel.y > 0 && canJump == false)
	{
		state = EntityState::FALLING;
	}

	if (vel.y < 0 && canJump == false)
	{
		state = EntityState::JUMPING;
	}




	//Ataque
	if (app->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN && !isAttacking && canJump) {
		isAttacking = true;
		pbody->body->SetLinearVelocity(b2Vec2(0, 0));
		if (isFacingLeft) {
			app->particles->AddParticle(app->particles->basicAttackL, position.x - 16, position.y);
		}
		else {
			app->particles->AddParticle(app->particles->basicAttackR, position.x + 16, position.y);
		}
	}

}

void Player::GodModeMovement(float dt)
{

	b2Vec2 vel;
	if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) {
		speedFast = speed * 2;
	}
	else {
		speedFast = speed;
	}


	vel = b2Vec2(0, 0);
	pbody->body->SetLinearVelocity(vel);
	pbody->body->GetFixtureList()[0].SetSensor(true);
	//Moverse a la izquierda
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		vel = b2Vec2(-speedFast * dt, pbody->body->GetLinearVelocity().y);
		isFacingLeft = true;
		state = EntityState::RUNNING;
		pbody->body->SetLinearVelocity(vel);
	}

	//Moverse a la derecha
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		vel = b2Vec2(speedFast * dt, pbody->body->GetLinearVelocity().y);
		isFacingLeft = false;
		state = EntityState::RUNNING;
		pbody->body->SetLinearVelocity(vel);
	}
	//Moverse arriba
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		vel = b2Vec2(pbody->body->GetLinearVelocity().x, -speedFast * dt);
		isFacingLeft = true;
		state = EntityState::RUNNING;
		pbody->body->SetLinearVelocity(vel);
	}

	//Moverse abajo
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		vel = b2Vec2(pbody->body->GetLinearVelocity().x, speedFast * dt);
		isFacingLeft = false;
		state = EntityState::RUNNING;
		pbody->body->SetLinearVelocity(vel);
	}

}

void Player::CameraMovement(float dt)
{

	/*CAMARA MIRAR ABAJO*/
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN) {
		watchUpDownTimer.Start();
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && watchUpDownTimer.ReadMSec() > 400) {
		yCameraOffset = -300;
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_UP && watchUpDownTimer.ReadMSec() > 400 || (abs(pbody->body->GetLinearVelocity().x) > 0)) {
		yCameraOffset = 0;
	}

	/*CAMARA MIRAR ARRIBA*/
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {
		watchUpDownTimer.Start();
	}
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && watchUpDownTimer.ReadMSec() > 400) {
		yCameraOffset = 300;
	}
	if ((app->input->GetKey(SDL_SCANCODE_W) == KEY_UP && watchUpDownTimer.ReadMSec() > 400) || (abs(pbody->body->GetLinearVelocity().x) > 0)) {
		yCameraOffset = 0;
	}


	uint windowH;
	uint windowW;
	app->win->GetWindowSize(windowW, windowH);


	int targetPosX;
	if (isFacingLeft) {
		targetPosX = (-position.x * app->win->GetScale() + (windowW / 2) - 10);
	}
	else {
		targetPosX = (-position.x * app->win->GetScale() + (windowW / 2) - 100);
	}

	int targetPosY = (-position.y * app->win->GetScale() + (windowH / 2) - 10) + yCameraOffset;

	targetPosY = MAX(targetPosY, -2980);
	targetPosX = MIN(targetPosX, -100);

	targetPosX += (isFacingLeft) ? 75 : -50;



	//El if este es un fix para el modo release
	if (app->GetFrameCount() < 20) {
		app->render->camera.x = lerp(app->render->camera.x, targetPosX, 1);
		app->render->camera.y = lerp(app->render->camera.y, targetPosY, 1);
	}
	else {
		app->render->camera.x = lerp(app->render->camera.x, targetPosX, dt * 0.005f);
		app->render->camera.y = lerp(app->render->camera.y, targetPosY, dt * 0.002f);
	}



}

void Player::OnCollision(PhysBody* physA, PhysBody* physB) {



	if (physA->ctype == ColliderType::PLAYER_FOOT) {
		numFootContacts++;
	}

	if (physA->ctype == ColliderType::PLAYER) {


		switch (physB->ctype)
		{
		case ColliderType::ITEM:
			//LOG("Collision ITEM");
			app->audio->PlayFx(pickCoinFxId);
			break;
		case ColliderType::CHEST:
			//LOG("Collision CHEST");
			app->audio->PlayFx(pickCoinFxId);
			((Chest*)physB->listener)->ChangeState(Chest::CHEST_STATE::OPENING);
			break;
		case ColliderType::PLATFORM:
			//LOG("Collision PLATFORM");
			break;
		case ColliderType::UNKNOWN:
			//LOG("Collision UNKNOWN");
			break;

		case ColliderType::COIN:
			app->audio->PlayFx(pickCoinFxId);
			app->entityManager->DestroyEntity(physB->listener);
			physB->body->SetActive(false);


			break;

		case ColliderType::DIE_HOLE:
		case ColliderType::SPYKES:
			if (!app->godMode) {
				isDying = true;
			}
			break;


		case ColliderType::PLATFORM_TRASPASS:
			colisionTraspassing.Add(physB);
			if (colisionTraspassing.Count() > 10) {
				colisionTraspassing.Del(colisionTraspassing.At(0));
			}
			break;

		case ColliderType::ENEMY:
			PhysBody* enemyBody = (PhysBody*)physB->body->GetUserData();
			if (enemyBody->listener->state == EntityState::ATTACKING) {
				LOG("DETECTA COLISION AU");
				if (invulnerableTimer.ReadMSec() > invulnerableTime * 1000) {
					LOG("RESTAR VIDA");
					invulnerableTimer.Start();
				}

			}
			break;
		}
	}


}

void Player::OnExitCollision(PhysBody* physA, PhysBody* physB)
{

	if (physA->ctype == ColliderType::PLAYER_FOOT) {
		numFootContacts--;
	}

	switch (physB->ctype)
	{
	case ColliderType::ITEM:
		break;
	case ColliderType::CHEST:

		break;
	case ColliderType::PLATFORM:
		break;
	case ColliderType::UNKNOWN:

		break;
	case ColliderType::PLATFORM_TRASPASS:
		break;
	}

}
