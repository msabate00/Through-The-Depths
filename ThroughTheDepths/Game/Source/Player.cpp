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
#include "Chest.h"

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");

	//Correr

	runAnim.PushBack({ 97, 1, 32, 32 });
	runAnim.PushBack({ 97, 33, 32, 32 });
	runAnim.PushBack({ 97, 65, 32, 32 });
	runAnim.PushBack({ 97, 97, 32, 32 });
	runAnim.PushBack({ 97, 129, 32, 32 });
	runAnim.PushBack({ 97, 161, 32, 32 });
	runAnim.PushBack({ 97, 193, 32, 32 });
	runAnim.PushBack({ 97, 225, 32, 32 });
	runAnim.speed = 0.17f;

	//Caminar

	walkAnim.PushBack({ 65, 1, 32, 32 });
	walkAnim.PushBack({ 65, 33, 32, 32 });
	walkAnim.PushBack({ 65, 65, 32, 32 });
	walkAnim.PushBack({ 65, 97, 32, 32 });
	walkAnim.speed = 0.17f;

	//Estar relajao

	idleAnim.PushBack({ 1, 1, 32, 32 });
	idleAnim.PushBack({ 1, 33, 32, 32 });
	idleAnim.speed = 0.17f;

	//Saltar

	jumpAnim.PushBack({ 161, 1, 32, 32 });
	jumpAnim.PushBack({ 161, 33, 32, 32 });
	jumpAnim.PushBack({ 161, 65, 32, 32 });
	jumpAnim.PushBack({ 161, 97, 32, 32 });
	jumpAnim.PushBack({ 161, 129, 32, 32 });
	jumpAnim.PushBack({ 161, 161, 32, 32 });
	jumpAnim.PushBack({ 161, 193, 32, 32 });
	jumpAnim.PushBack({ 161, 225, 32, 32 });
	jumpAnim.speed = 0.17f;

	//CAMBIAR jumpAnim, y dividirlo en jumpUpAnim y jumpDownAnim;




	//Morir Desvanecido

	ghostAnim.PushBack({ 193, 1, 32, 32 });
	ghostAnim.PushBack({ 193, 33, 32, 32 });
	ghostAnim.PushBack({ 193, 65, 32, 32 });
	ghostAnim.speed = 0.17f;

	//Morir

	dieAnim.PushBack({ 225, 1, 32, 32 });
	dieAnim.PushBack({ 225, 33, 32, 32 });
	dieAnim.PushBack({ 225, 65, 32, 32 });
	dieAnim.PushBack({ 225, 97, 32, 32 });
	dieAnim.PushBack({ 225, 129, 32, 32 });
	dieAnim.PushBack({ 225, 161, 32, 32 });
	dieAnim.PushBack({ 225, 193, 32, 32 });
	dieAnim.PushBack({ 225, 225, 32, 32 });
	dieAnim.speed = 0.17f;

	//Atacar
	
	attackAnim.PushBack({ 257, 1, 32, 32 });
	attackAnim.PushBack({ 257, 33, 32, 32 });
	attackAnim.PushBack({ 257, 65, 32, 32 });
	attackAnim.PushBack({ 257, 97, 32, 32 });
	attackAnim.PushBack({ 257, 129, 32, 32 });
	attackAnim.PushBack({ 257, 161, 32, 32 });
	attackAnim.PushBack({ 257, 193, 32, 32 });
	attackAnim.PushBack({ 257, 225, 32, 32 });
	attackAnim.speed = 0.17f;
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

	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 16,  bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;
	pbody->body->SetFixedRotation(true);
	

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


	b2Vec2 vel = b2Vec2(0,  pbody->body->GetLinearVelocity().y);
	

	LOG("TraspasingCol %d, tiempo: %d, cantidad: %d", traspassingColision, traspassingTimer.ReadSec(), colisionTraspassing.Count());

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


	
	vel.y -= GRAVITY_Y;
	pbody->body->SetLinearVelocity(vel);
	
	//Sistema de salto
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && app->input->GetKey(SDL_SCANCODE_S) != KEY_REPEAT) {
		vel.y = 0;
		pbody->body->SetLinearVelocity(vel);
		pbody->body->ApplyLinearImpulse(b2Vec2(0, GRAVITY_Y * jumpForce), pbody->body->GetWorldCenter(), true);
		
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



	if (app->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN) {
		dying = !dying;
	}


	if (dying) {
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
		LOG("Collision traspass count: %d", colisionTraspassing.Count());
	


		break;
	}
}