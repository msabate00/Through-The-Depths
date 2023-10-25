#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Chest.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene() : Module()
{
	name.Create("scene");
	
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	configScene = &config;

	// iterate all objects in the scene
	// Check https://pugixml.org/docs/quickstart.html#access
	//for (pugi::xml_node itemNode = config.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	//{
	//	Item* item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
	//	item->parameters = itemNode;
	//}

	//for (pugi::xml_node chestNode = config.child("chest"); chestNode; chestNode = chestNode.next_sibling("chest"))
	//{
	//	Chest* chest = (Chest*)app->entityManager->CreateEntity(EntityType::CHEST);
	//	chest->parameters = chestNode;
	//	LOG("AAA");
	//}

	/*if (config.child("player")) {
		player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
		player->parameters = config.child("player");
	}*/

	

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	fondo0 = app->tex->Load("Assets/Maps/tilesets/background_0_32x32.png");
	fondo1 = app->tex->Load("Assets/Maps/tilesets/background_1_32x32.png");
	fondo2 = app->tex->Load("Assets/Maps/tilesets/background_2_32x32.png");
	
	// NOTE: We have to avoid the use of paths in the code, we will move it later to a config file
	//img = app->tex->Load("Assets/Textures/test.png");
	
	//Music is commented so that you can add your own music
	//app->audio->PlayMusic("Assets/Audio/Music/music_spy.ogg");

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	//Get the size of the texture
	app->tex->GetSize(img, texW, texH);

	textPosX = (float)windowW / 2 - (float)texW / 2;
	textPosY = (float)windowH / 2 - (float)texH / 2;

	app->map->Load();

	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
		app->map->mapData.width,
		app->map->mapData.height,
		app->map->mapData.tileWidth,
		app->map->mapData.tileHeight,
		app->map->mapData.tilesets.Count());

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	float camSpeed = 1; 

	if(app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y += (int)ceil(camSpeed * dt);

	if(app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y -= (int)ceil(camSpeed * dt);

	if(app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x += (int)ceil(camSpeed * dt);

	if(app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x -= (int)ceil(camSpeed * dt);


	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
		//player = nullptr;
		//app->entityManager->DestroyAllEntities();
		/*app->map->CleanUp();*/
		//app->map->Load();
	}

	
	SDL_Rect fondo0x{ 0,0, 576, 360 };
	SDL_Rect fondo0y{ 0,0, 576, 360 };

	
	int alturaFondos = 282;



	//FONDO 0
	float paralaxSpeed = 0.2f;

	app->render->DrawTexture(fondo0, fondo0Offset, alturaFondos, SDL_FLIP_NONE, &fondo0x, paralaxSpeed);
	app->render->DrawTexture(fondo0, fondo0Offset + 500, alturaFondos, SDL_FLIP_NONE, &fondo0y, paralaxSpeed);
	app->render->DrawTexture(fondo0, fondo0Offset + 1000, alturaFondos, SDL_FLIP_NONE, &fondo0y, paralaxSpeed);
	app->render->DrawTexture(fondo0, fondo0Offset - 500, alturaFondos, SDL_FLIP_NONE, &fondo0y, paralaxSpeed);
	app->render->DrawTexture(fondo0, fondo0Offset - 1000, alturaFondos, SDL_FLIP_NONE, &fondo0y, paralaxSpeed);


	

	int posJugadorParalax = (int)(player->position.x * 0.2f);
	int posicionFondos = 500;
	int posicionJugadorOffset = 200;


	if (posJugadorParalax + posicionJugadorOffset > fondo0Offset + posicionFondos) {
		fondo0Offset += posicionFondos;
	}
	if (posJugadorParalax + posicionJugadorOffset < fondo0Offset + posicionFondos) {
		fondo0Offset -= posicionFondos;
	}


	//FONDO1

	paralaxSpeed = 0.4f;

	app->render->DrawTexture(fondo1, fondo1Offset, alturaFondos, SDL_FLIP_NONE, &fondo0x, paralaxSpeed);
	app->render->DrawTexture(fondo1, fondo1Offset + 500, alturaFondos, SDL_FLIP_NONE, &fondo0y, paralaxSpeed);
	app->render->DrawTexture(fondo1, fondo1Offset + 1000, alturaFondos, SDL_FLIP_NONE, &fondo0y, paralaxSpeed);
	app->render->DrawTexture(fondo1, fondo1Offset - 500, alturaFondos, SDL_FLIP_NONE, &fondo0y, paralaxSpeed);
	app->render->DrawTexture(fondo1, fondo1Offset - 1000, alturaFondos, SDL_FLIP_NONE, &fondo0y, paralaxSpeed);




	posJugadorParalax = (int)(player->position.x * 0.4f);
	posicionFondos = 500;
	posicionJugadorOffset = 200;


	if (posJugadorParalax + posicionJugadorOffset > fondo1Offset + posicionFondos) {
		fondo1Offset += posicionFondos;
	}
	if (posJugadorParalax + posicionJugadorOffset < fondo1Offset + posicionFondos) {
		fondo1Offset -= posicionFondos;
	}

	
	//FONDO2
	paralaxSpeed = 0.6f;
	app->render->DrawTexture(fondo2, fondo2Offset, alturaFondos, SDL_FLIP_NONE, &fondo0x, paralaxSpeed);
	app->render->DrawTexture(fondo2, fondo2Offset + 500, alturaFondos, SDL_FLIP_NONE, &fondo0y, paralaxSpeed);
	app->render->DrawTexture(fondo2, fondo2Offset + 1000, alturaFondos, SDL_FLIP_NONE, &fondo0y, paralaxSpeed);
	app->render->DrawTexture(fondo2, fondo2Offset - 500, alturaFondos, SDL_FLIP_NONE, &fondo0y, paralaxSpeed);
	app->render->DrawTexture(fondo2, fondo2Offset - 1000, alturaFondos, SDL_FLIP_NONE, &fondo0y, paralaxSpeed);




	posJugadorParalax = (int)(player->position.x * 0.2f);
	posicionFondos = 500;
	posicionJugadorOffset = 200;


	if (posJugadorParalax + posicionJugadorOffset > fondo2Offset + posicionFondos) {
		fondo2Offset += posicionFondos;
	}
	if (posJugadorParalax + posicionJugadorOffset < fondo2Offset + posicionFondos) {
		fondo2Offset -= posicionFondos;
	}


	


	/*SDL_Rect fondo1x{ 0,0, 576, 360 };
	SDL_Rect fondo1y{ 0,0, 576, 360 };

	app->render->DrawTexture(fondo1, 0, 563, SDL_FLIP_NONE, &fondo1x, 0.4f);
	app->render->DrawTexture(fondo1, 600, 563, SDL_FLIP_NONE, &fondo1y, 0.4f);*/

	/*SDL_Rect fondo2x{ 0,0, 576, 360 };
	SDL_Rect fondo2y{ 0,0, 576, 360 };

	app->render->DrawTexture(fondo2, 0, 845, SDL_FLIP_NONE, &fondo2x, 0.6f);
	app->render->DrawTexture(fondo2, 360, 845, SDL_FLIP_NONE, &fondo2y, 0.6f);*/

	// Renders the image in the center of the screen 
	//app->render->DrawTexture(img, (int)textPosX, (int)textPosY);

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	
	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

Player* Scene::getPlayer()
{
	return player;
}

void Scene::setPlayer(Player* new_player)
{
	player = new_player;

}
