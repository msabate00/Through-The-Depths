#include "wallBoss.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

WallBoss::WallBoss() : Entity(EntityType::COIN)
{
	name.Create("wallBoss");

	


}

WallBoss::~WallBoss() {}

bool WallBoss::Awake() {

	
	/*switch (parameters.attribute("state").as_int())
	{
		case 1:
			state = Coin_STATE::CLOSED;
			break;
		case 2:
			state = Coin_STATE::OPENING;
			break;
		case 3:
			state = Coin_STATE::OPENED;
			break;
		default:
			state = Coin_STATE::CLOSED;
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

	
	

	idleAnim.LoadAnimation("wallBoss", "idleAnim");
	appearAnim.LoadAnimation("wallBoss", "appearAnim");
	


	return true;
}

bool WallBoss::Start() {

	//initilize textures
	//position.x = parameters.attribute("x").as_int();
	//position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	texture = app->tex->Load(texturePath);

	pbody = app->physics->CreateRectangle(position.x + 16, position.y - 96, 32, 224, bodyType::STATIC);
	pbody->body->SetActive(false);
	pbody->ctype = ColliderType::PLATFORM;
	pbody->listener = this;
	
	return true;
}

bool WallBoss::Update(float dt)
{
	currentAnimation = &idleAnim;
	
	if (enterZone) {
		currentAnimation = &appearAnim;
		if (!pbody->body->IsActive()) {
			pbody->body->SetActive(true);
		}
	}



	currentAnimation->Update();

	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	

	return true;
}

bool WallBoss::PostUpdate() {

	if (currentAnimation == nullptr) { currentAnimation = &idleAnim; }
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y - 96, SDL_FLIP_NONE, &rect);


	return true;
}

bool WallBoss::CleanUp()
{
	app->physics->GetWorld()->DestroyBody(pbody->body);
	SDL_DestroyTexture(texture);
	return true;
}
