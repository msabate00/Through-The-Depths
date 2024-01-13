#include "Food.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

Food::Food() : Entity(EntityType::FOOD)
{
	name.Create("Food");

}

Food::~Food() {}

bool Food::Awake() {

	
	/*switch (parameters.attribute("state").as_int())
	{
		case 1:
			state = Food_STATE::CLOSED;
			break;
		case 2:
			state = Food_STATE::OPENING;
			break;
		case 3:
			state = Food_STATE::OPENED;
			break;
		default:
			state = Food_STATE::CLOSED;
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

	
	idleAnim.PushBack({0, 288, 32, 32});
	idleAnim.PushBack({ 32, 288, 32, 32 });
	idleAnim.PushBack({ 64, 288, 32, 32 });
	idleAnim.PushBack({ 96, 288, 32, 32 });


	idleAnim.loop = true;
	idleAnim.pingpong = true;
	idleAnim.speed = 0.1f;



	return true;
}

bool Food::Start() {

	//initilize textures
	//position.x = parameters.attribute("x").as_int();
	//position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	texture = app->tex->Load(texturePath);

	pbody = app->physics->CreateRectangleSensor(position.x + 16, position.y, 32, 32, bodyType::STATIC);
	pbody->ctype = ColliderType::FOOD;
	pbody->listener = this;
	
	return true;
}

bool Food::Update(float dt)
{
	currentAnimation = &idleAnim;
	



	currentAnimation->Update();

	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	

	return true;
}

bool Food::PostUpdate() {

	if (currentAnimation == nullptr) { currentAnimation = &idleAnim; }
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, SDL_FLIP_NONE, &rect);


	return true;
}

bool Food::CleanUp()
{
	app->physics->GetWorld()->DestroyBody(pbody->body);
	SDL_DestroyTexture(texture);
	return true;
}
