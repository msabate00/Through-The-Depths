#include "Chest.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Chest::Chest() : Entity(EntityType::CHEST)
{
	name.Create("chest");

	


}

Chest::~Chest() {}

bool Chest::Awake() {

	
	switch (parameters.attribute("state").as_int())
	{
		case 1:
			state = CHEST_STATE::CLOSED;
			break;
		case 2:
			state = CHEST_STATE::OPENING;
			break;
		case 3:
			state = CHEST_STATE::OPENED;
			break;
		default:
			state = CHEST_STATE::CLOSED;
			break;
	}


	closedAnim.PushBack({0, 4, 34, 28});


	openingAnim.PushBack({ 0, 4, 34, 28 });
	openingAnim.PushBack({ 64, 4, 34, 28 });
	openingAnim.PushBack({ 128, 4, 34, 28 });
	openingAnim.PushBack({ 192, 4, 34, 28 });
	openingAnim.PushBack({ 256, 4, 34, 28 });
	openingAnim.loop = false;
	openingAnim.speed = 0.1f;


	openedAnim.PushBack({ 256, 4, 34, 28 });




	return true;
}

bool Chest::Start() {

	//initilize textures
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	texture = app->tex->Load(texturePath);
	pbody = app->physics->CreateRectangle(position.x + 16, position.y + 16, 30, 27, bodyType::STATIC);
	pbody->ctype = ColliderType::CHEST;
	pbody->listener = this;

	return true;
}

bool Chest::Update(float dt)
{

	switch (state)
	{
	case Chest::CHEST_STATE::CLOSED:
		currentAnimation = &closedAnim;
		break;
	case Chest::CHEST_STATE::OPENING:
		currentAnimation = &openingAnim;
		break;
	case Chest::CHEST_STATE::OPENED:
		currentAnimation = &openedAnim;
		break;
	default:
		break;
	}



	currentAnimation->Update();

	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	

	return true;
}

bool Chest::PostUpdate() {


	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, SDL_FLIP_NONE, &rect);


	if (state == CHEST_STATE::OPENING && currentAnimation->HasFinished()) {
		state = CHEST_STATE::OPENED;
	}


	return true;
}

bool Chest::CleanUp()
{
	return true;
}

void Chest::ChangeState(CHEST_STATE new_state) {

	state = new_state;

}