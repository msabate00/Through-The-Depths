#include "SaveStatue.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

SaveStatue::SaveStatue() : Entity(EntityType::SAVE_STATUE)
{
	name.Create("SaveStatue");

	


}

SaveStatue::~SaveStatue() {}

bool SaveStatue::Awake() {

	
	/*switch (parameters.attribute("state").as_int())
	{
		case 1:
			state = SaveStatue_STATE::CLOSED;
			break;
		case 2:
			state = SaveStatue_STATE::OPENING;
			break;
		case 3:
			state = SaveStatue_STATE::OPENED;
			break;
		default:
			state = SaveStatue_STATE::CLOSED;
			break;
	}*/


	/*closedAnim.PushBack({0, 4, 34, 28});


	openingAnim.PushBack({ 0, 4, 34, 28 });
	openingAnim.PushBack({ 64, 4, 34, 28 });
	openingAnim.PushBack({ 128, 4, 34, 28 });
	openingAnim.PushBack({ 192, 4, 34, 28 });
	openingAnim.PushBack({ 256, 4, 34, 28 });
	openingAnim.loop = false;
	openingAnim.speed = 0.1f;


	openedAnim.PushBack({ 256, 4, 34, 28 });*/

	
	idleAnim.PushBack({0, 193, 64, 96});
	idleAnim.loop = true;
	idleAnim.speed = 0.1f;

	savedAnim.PushBack({ 64, 193, 64, 96 });
	idleAnim.loop = true;
	idleAnim.speed = 0.1f;



	return true;
}

bool SaveStatue::Start() {

	//initilize textures
	//position.x = parameters.attribute("x").as_int();
	//position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	texture = app->tex->Load(texturePath);

	pbody = app->physics->CreateRectangleSensor(position.x, position.y, 32, 92, bodyType::STATIC);
	pbody->ctype = ColliderType::SAVE_STATUE;
	pbody->listener = this;
	
	return true;
}

bool SaveStatue::Update(float dt)
{
	currentAnimation = &idleAnim;
	if (saved) {
		currentAnimation = &savedAnim;
	}



	currentAnimation->Update();

	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	

	return true;
}

bool SaveStatue::PostUpdate() {

	if (currentAnimation == nullptr) { currentAnimation = &idleAnim; }
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x-16, position.y - 30, SDL_FLIP_NONE, &rect);


	return true;
}

bool SaveStatue::CleanUp()
{
	app->physics->GetWorld()->DestroyBody(pbody->body);
	SDL_DestroyTexture(texture);
	return true;
}
