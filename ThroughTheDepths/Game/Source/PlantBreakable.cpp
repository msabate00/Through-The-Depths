#include "PlantBreakable.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

PlantBreakable::PlantBreakable() : Entity(EntityType::PLANT_BREAKABLE)
{
	name.Create("PlantBreakable");

	


}

PlantBreakable::~PlantBreakable() {}

bool PlantBreakable::Awake() {

	
	/*switch (parameters.attribute("state").as_int())
	{
		case 1:
			state = PlantBreakable_STATE::CLOSED;
			break;
		case 2:
			state = PlantBreakable_STATE::OPENING;
			break;
		case 3:
			state = PlantBreakable_STATE::OPENED;
			break;
		default:
			state = PlantBreakable_STATE::CLOSED;
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

	
	idleAnim.PushBack({0, 96, 32, 33});

	damagedAnim.PushBack({ 0, 96, 32, 33 });
	damagedAnim.PushBack({ 32, 96, 32, 33 });
	damagedAnim.PushBack({ 32+32, 96, 32, 33 });
	damagedAnim.PushBack({ 32+32+32, 96, 32, 33 });
	damagedAnim.loop = false;
	damagedAnim.speed = 0.8;



	return true;
}

bool PlantBreakable::Start() {

	//initilize textures
	//position.x = parameters.attribute("x").as_int();
	//position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	texture = app->tex->Load(texturePath);

	pbody = app->physics->CreateRectangle(position.x + 16, position.y, 15, 32, bodyType::STATIC);
	pbody->ctype = ColliderType::PLANT_BREAKABLE;
	pbody->listener = this;

	return true;
}

bool PlantBreakable::Update(float dt)
{
	currentAnimation = &idleAnim;
	
	if (isDamaged) {
		currentAnimation = &damagedAnim;
		if (damagedAnim.HasFinished()) {
			isDamaged = false;
			damagedAnim.Reset();
		}
	}



	currentAnimation->Update();

	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	

	return true;
}

bool PlantBreakable::PostUpdate() {

	if (currentAnimation == nullptr) { currentAnimation = &idleAnim; }
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, SDL_FLIP_NONE, &rect);


	return true;
}

bool PlantBreakable::CleanUp()
{
	return true;
}

void PlantBreakable::OnCollision(PhysBody* physA, PhysBody* physB) {

	

	if (physB->ctype == ColliderType::PLAYER_PROYECTILE) {	
		if (!isDamaged) {
			life--;
			isDamaged = true;
			if (life <= 0) {
				pbody->body->GetWorld()->DestroyBody(pbody->body);
				pbody->body->SetActive(false);
				pbody->listener = nullptr;
				pbody->body->SetTransform(b2Vec2(-10000, -10000), 0);
				app->entityManager->DestroyEntity(this);
			}
		}
		
		
	}

}
