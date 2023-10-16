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
#include "Chest.h"

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");

	//Correr

	runAnim.PushBack({ 1, 97,  32, 32 });
	runAnim.PushBack({ 33,97,  32, 32 });
	runAnim.PushBack({ 65, 97, 32, 32 });
	runAnim.PushBack({ 97, 97, 32, 32 });
	runAnim.PushBack({ 129, 97, 32, 32 });
	runAnim.PushBack({ 161, 97, 32, 32 });
	runAnim.PushBack({ 193, 97, 32, 32 });
	runAnim.PushBack({ 225, 97, 32, 32 });
	runAnim.speed = 0.17f;

	//Caminar

	walkAnim.PushBack({ 1, 65, 32, 32 });
	walkAnim.PushBack({ 33, 65, 32, 32 });
	walkAnim.PushBack({ 65, 65, 32, 32 });
	walkAnim.PushBack({ 97, 65, 32, 32 });
	walkAnim.speed = 0.17f;

	//Estar relajao

	idleAnim.PushBack({ 1, 1, 32, 32 });
	idleAnim.PushBack({ 33, 1, 32, 32 });
	idleAnim.speed = 0.1f;

	//Saltar

	jumpUpAnim.PushBack({ 1, 161, 32, 32 });
	jumpUpAnim.PushBack({ 33, 161, 32, 32 });
	jumpUpAnim.PushBack({ 65, 161, 32, 32 });
	jumpUpAnim.PushBack({ 97, 161, 32, 32 });
	jumpDownAnim.PushBack({ 129, 161, 32, 32 });
	jumpDownAnim.PushBack({ 161, 161, 32, 32 });
	jumpDownAnim.PushBack({ 193, 161, 32, 32 });
	jumpDownAnim.PushBack({ 225, 161, 32, 32 });
	
	jumpDownAnim.speed = 0.17f;
	jumpUpAnim.speed = 0.17f;
	//CAMBIAR jumpAnim, y dividirlo en jumpUpAnim y jumpDownAnim; La de up va con loop = false; la de bajar va con loop= true;




	//Morir Desvanecido

	ghostAnim.PushBack({ 1, 193, 32, 32 });
	ghostAnim.PushBack({ 33, 193, 32, 32 });
	ghostAnim.PushBack({ 65, 193, 32, 32 });
	ghostAnim.speed = 0.17f;

	//Morir

	dieAnim.PushBack({ 1, 225, 32, 32 });
	dieAnim.PushBack({ 33, 225, 32, 32 });
	dieAnim.PushBack({ 65, 225, 32, 32 });
	dieAnim.PushBack({ 97, 225, 32, 32 });
	dieAnim.PushBack({ 129, 225, 32, 32 });
	dieAnim.PushBack({ 161, 225, 32, 32 });
	dieAnim.PushBack({ 193, 225, 32, 32 });
	dieAnim.PushBack({ 225, 225, 32, 32 });
	dieAnim.speed = 0.17f;

	//Atacar
	
	attackAnim.PushBack({ 1, 257, 32, 32 });
	attackAnim.PushBack({ 33, 257, 32, 32 });
	attackAnim.PushBack({ 65, 257, 32, 32 });
	attackAnim.PushBack({ 97, 257, 32, 32 });
	attackAnim.PushBack({ 129, 257, 32, 32 });
	attackAnim.PushBack({ 161, 257, 32, 32 });
	attackAnim.PushBack({ 193, 257, 32, 32 });
	attackAnim.PushBack({ 225, 257, 32, 32 });
	attackAnim.speed = 0.35f;
	attackAnim.loop = false;
}

Player::~Player() {

}

bool Player::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Player::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	int points = 2;
	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 16,  bodyType::DYNAMIC);
	//pbody = app->physics->CreateRectangle(position.x + 16, position.y + 16, 32, 32,  bodyType::DYNAMIC);
	
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;
	pbody->body->SetFixedRotation(true);


	/*pbodyFoot = app->physics->CreateRectangleSensor(position.x + 16, position.y + 16, 16, 3, bodyType::DYNAMIC);

	pbodyFoot->listener = this;
	pbodyFoot->ctype = ColliderType::PLAYER_FOOT;
	pbodyFoot->body->SetFixedRotation(true);*/
	
	

	pickCoinFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");







	uint windowH;
	uint windowW;
	app->win->GetWindowSize(windowW, windowH);

	app->render->camera.x = (-position.x * app->win->GetScale() + (windowW / 2));
	app->render->camera.y = (-position.y * app->win->GetScale() + (windowH / 2));

	return true;
}

bool Player::Update(float dt)
{
	currentAnimation = &idleAnim;

	b2Vec2 positionFoot;
	positionFoot.x = PIXEL_TO_METERS(position.x+0.3);
	//positionFoot.y = PIXEL_TO_METERS(position.y + 0.67);
	positionFoot.y = PIXEL_TO_METERS(position.y + 1);

	//pbodyFoot->body->SetTransform(positionFoot, pbodyFoot->body->GetAngle());


	b2Vec2 vel = b2Vec2(0,  pbody->body->GetLinearVelocity().y);
	

	//LOG("TraspasingCol %d, tiempo: %d, cantidad: %d", traspassingColision, traspassingTimer.ReadSec(), colisionTraspassing.Count());

	if (traspassingColision && traspassingTimer.ReadMSec() > 100) {
		while (colisionTraspassing.Count() != 0) {
			colisionTraspassing[0]->body->SetActive(true);
			colisionTraspassing.Del(colisionTraspassing.At(0));
		}
		
		traspassingColision = false;
	}
	


	
	//Moverse a la izquierda
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		vel = b2Vec2(-speed*dt, pbody->body->GetLinearVelocity().y);
		isFacingLeft = true;
		currentAnimation = &runAnim;
	}

	//Moverse a la derecha
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		vel = b2Vec2(speed*dt, pbody->body->GetLinearVelocity().y);
		isFacingLeft = false;
		currentAnimation = &runAnim;
	}

	//Atraves plataformas traspasables
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT) {
		
		traspassingColision = true;
		traspassingTimer.Start();

		for (int i = 0; i < colisionTraspassing.Count(); i++) {
			colisionTraspassing[i]->body->SetActive(false);
		}

	}

	//Ataque
	if (app->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN && !isAttacking) {
		isAttacking = true;
		if (isFacingLeft) {
			app->particles->AddParticle(app->particles->basicAttackL, position.x - 16, position.y);
		}
		else {
			app->particles->AddParticle(app->particles->basicAttackR, position.x + 16, position.y);
		}
		

	}

	


	
	vel.y -= GRAVITY_Y;
	pbody->body->SetLinearVelocity(vel);
	
	//Sistema de salto
	if (!canJump) {
		canJump = (abs(pbody->body->GetLinearVelocity().y) == 1.0f);
	}
	
	
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && app->input->GetKey(SDL_SCANCODE_S) != KEY_REPEAT && canJump) {
		vel.y = 0;
		pbody->body->SetLinearVelocity(vel);
		pbody->body->ApplyLinearImpulse(b2Vec2(0, GRAVITY_Y * jumpForce), pbody->body->GetWorldCenter(), true);
		canJump = false;
		
		
	}


	
	

	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;



	//Movimiento de la camara
	if (!app->debug) {
		uint windowH;
		uint windowW;
		app->win->GetWindowSize(windowW, windowH);

		//app->render->camera.x = (-position.x * app->win->GetScale() + (windowW / 2));
		int targetPosX = (-position.x * app->win->GetScale() + (windowW / 2) - 10);
		int targetPosY = (-position.y * app->win->GetScale() + (windowH / 2) - 10);

		
		
		targetPosX += (isFacingLeft) ? 75 : -50;

		app->render->camera.x = lerp(app->render->camera.x,targetPosX, dt * 0.01f);
		app->render->camera.y = lerp(app->render->camera.y,targetPosY, dt * 0.002f);
		//app->render->camera.x = (-position.x * app->win->GetScale() + (windowW / 2));
	}



	if (isAttacking) {
		currentAnimation = &attackAnim;

		if (attackAnim.HasFinished()) {
			attackAnim.Reset();
			isAttacking = false;
		}
	}



	if (app->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN) {
		isDying = !isDying;
	}


	if (isDying) {
		currentAnimation = &dieAnim;
	}



	currentAnimation->Update();

	return true;
}

bool Player::PostUpdate() {



	SDL_Rect rect = currentAnimation->GetCurrentFrame();

	if (isFacingLeft) {
		app->render->DrawTexture(texture, position.x, position.y, SDL_FLIP_HORIZONTAL, &rect);
	}
	else {
		app->render->DrawTexture(texture, position.x, position.y, SDL_FLIP_NONE, &rect);
	}



	
	return true;
}

bool Player::CleanUp()
{

	return true;
}

void Player::OnCollision(PhysBody* physA, PhysBody* physB) {



	if (physA->ctype == ColliderType::PLAYER_FOOT) {
		numFootContacts++;
	}
	
	if(physA->ctype == ColliderType::PLAYER){


		switch (physB->ctype)
		{
		case ColliderType::ITEM:
			LOG("Collision ITEM");
			app->audio->PlayFx(pickCoinFxId);
			break;
		case ColliderType::CHEST:
			LOG("Collision CHEST");
			app->audio->PlayFx(pickCoinFxId);
			((Chest*)physB->listener)->ChangeState(Chest::CHEST_STATE::OPENING);
			break;
		case ColliderType::PLATFORM:
			LOG("Collision PLATFORM");
			break;
		case ColliderType::UNKNOWN:
			LOG("Collision UNKNOWN");
			break;
		case ColliderType::PLATFORM_TRASPASS:

			colisionTraspassing.Add(physB);
			if (colisionTraspassing.Count() > 10) {
				colisionTraspassing.Del(colisionTraspassing.At(0));
			}
			



			break;
		}
	}

	
}

void Player::OnExitCollision(PhysBody* physA, PhysBody* physB)
{

	if (physA->ctype == ColliderType::PLAYER_FOOT) {
		numFootContacts--;
		//LOG("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
	}

	

	/*switch (physB->ctype)
	{
	case ColliderType::ITEM:
		LOG("FUERDA DE Collision ITEM");
		app->audio->PlayFx(pickCoinFxId);
		break;
	case ColliderType::CHEST:
		LOG("FUERDA DE Collision CHEST");
		app->audio->PlayFx(pickCoinFxId);
		((Chest*)physB->listener)->ChangeState(Chest::CHEST_STATE::OPENING);
		break;
	case ColliderType::PLATFORM:
		LOG("FUERDA DE Collision PLATFORM");
		break;
	case ColliderType::UNKNOWN:
		LOG("FUERDA DE Collision UNKNOWN");
		break;
	case ColliderType::PLATFORM_TRASPASS:

		colisionTraspassing.Add(physB);
		if (colisionTraspassing.Count() > 10) {
			colisionTraspassing.Del(colisionTraspassing.At(0));
		}
		LOG("FUERDA DE Collision traspass count: %d", colisionTraspassing.Count());



		break;
	}*/

}
