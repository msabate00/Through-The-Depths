#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");
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

	pbody = app->physics->CreateRectangle(position.x, position.y, 30,30,  bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;
	pbody->body->SetFixedRotation(true);
	

	pickCoinFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");

	return true;
}

bool Player::Update(float dt)
{



	b2Vec2 vel = b2Vec2(0,  pbody->body->GetLinearVelocity().y);

	//float* a;
	//float* b;

	//int valor = pbody->RayCast(position.x, position.y, position.x, position.y + 10, &a, &b);
	//
	//LOG("DEBAJO HAY: %d", );
	


	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		//
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		//
	}

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		vel = b2Vec2(-speed*dt, pbody->body->GetLinearVelocity().y);
		isFacingLeft = true;
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		vel = b2Vec2(speed*dt, pbody->body->GetLinearVelocity().y);
		isFacingLeft = false;
	}

	
	vel.y -= GRAVITY_Y;
	pbody->body->SetLinearVelocity(vel);
	//NUEVO
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		vel.y = 0;
		pbody->body->SetLinearVelocity(vel);
		pbody->body->ApplyLinearImpulse(b2Vec2(0, GRAVITY_Y * jumpForce), pbody->body->GetWorldCenter(), true);
		
	}


	//Set the velocity of the pbody of the player
	

	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	

	return true;
}

bool Player::PostUpdate() {

	if (isFacingLeft) {
		app->render->DrawTexture(texture, position.x, position.y, SDL_FLIP_HORIZONTAL);
	}
	else {
		app->render->DrawTexture(texture, position.x, position.y, SDL_FLIP_NONE);
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
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}
}