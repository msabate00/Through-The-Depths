#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Chest.h"
#include "FadeToBlack.h"
#include "GuiManager.h"
#include "Interface.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene(bool start_enabled) : Module(start_enabled)
{
	name.Create("scenee");
	
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	//configScene = &config;
	
	

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
	pugi::xml_document configFile;
	pugi::xml_node sceneNode;
	pugi::xml_parse_result parseResult = configFile.load_file("config.xml");
	sceneNode = configFile.child("config").child("scene");


	char* nombreNivel;

	switch (app->sceneLevel)
	{
	case 0:
		fondo0 = app->tex->Load(sceneNode.child("textures").child("fondo0Bosque").attribute("texturepath").as_string());
		fondo1 = app->tex->Load(sceneNode.child("textures").child("fondo1Bosque").attribute("texturepath").as_string());
		fondo2 = app->tex->Load(sceneNode.child("textures").child("fondo2Bosque").attribute("texturepath").as_string());
		break;
	case 1:
		fondo0 = app->tex->Load(sceneNode.child("textures").child("fondo0Pueblo").attribute("texturepath").as_string());
		fondo1 = app->tex->Load(sceneNode.child("textures").child("fondo1Pueblo").attribute("texturepath").as_string());
		fondo2 = app->tex->Load(sceneNode.child("textures").child("fondo2Pueblo").attribute("texturepath").as_string());
		break;
	default:
		fondo0 = app->tex->Load(sceneNode.child("textures").child("fondo0Bosque").attribute("texturepath").as_string());
		fondo1 = app->tex->Load(sceneNode.child("textures").child("fondo1Bosque").attribute("texturepath").as_string());
		fondo2 = app->tex->Load(sceneNode.child("textures").child("fondo2Bosque").attribute("texturepath").as_string());
		break;
	}


	
	
	

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
	//app->LoadRequest();
	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
		app->map->mapData.width,
		app->map->mapData.height,
		app->map->mapData.tileWidth,
		app->map->mapData.tileHeight,
		app->map->mapData.tilesets.Count());


	/*SDL_Rect btPos = { windowW / 2 - 60, windowH / 2 - 10, 120,20 };
	gcButtom = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "MyButton", btPos, this);*/



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

	
	
	SDL_Rect fondo0x{ 0,0, 576, 1760 };
	SDL_Rect fondo0y{ 0,0, 576, 1760 };
	SDL_Rect fondo1x{ 0,0, 576, 360 };
	SDL_Rect fondo1y{ 0,0, 576, 360 };
	SDL_Rect fondo2x{ 0,0, 576, 360 };
	SDL_Rect fondo2y{ 0,0, 576, 360 };
	
	int alturaFondo0 = -1150;
	int alturaFondo1 = 550;
	int alturaFondo2 = 850;


	//FONDO 0
	float paralaxSpeed = 0.2f;



	app->render->DrawTexture(fondo0, fondo0Offset, alturaFondo0, SDL_FLIP_NONE, &fondo0x, paralaxSpeed);
	app->render->DrawTexture(fondo0, fondo0Offset + 500, alturaFondo0, SDL_FLIP_NONE, &fondo0y, paralaxSpeed);
	app->render->DrawTexture(fondo0, fondo0Offset + 1000, alturaFondo0, SDL_FLIP_NONE, &fondo0y, paralaxSpeed);
	app->render->DrawTexture(fondo0, fondo0Offset - 500, alturaFondo0, SDL_FLIP_NONE, &fondo0y, paralaxSpeed);
	app->render->DrawTexture(fondo0, fondo0Offset - 1000, alturaFondo0, SDL_FLIP_NONE, &fondo0y, paralaxSpeed);


	
	
	int posJugadorParalax = (int)(player->position.x * paralaxSpeed);
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

	app->render->DrawTexture(fondo1, fondo1Offset, alturaFondo1, SDL_FLIP_NONE, &fondo1x, paralaxSpeed);
	app->render->DrawTexture(fondo1, fondo1Offset + 500, alturaFondo1, SDL_FLIP_NONE, &fondo1y, paralaxSpeed);
	app->render->DrawTexture(fondo1, fondo1Offset + 1000, alturaFondo1, SDL_FLIP_NONE, &fondo1y, paralaxSpeed);
	app->render->DrawTexture(fondo1, fondo1Offset - 500, alturaFondo1, SDL_FLIP_NONE, &fondo1y, paralaxSpeed);
	app->render->DrawTexture(fondo1, fondo1Offset - 1000, alturaFondo1, SDL_FLIP_NONE, &fondo1y, paralaxSpeed);




	posJugadorParalax = (int)(player->position.x * paralaxSpeed);
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
	app->render->DrawTexture(fondo2, fondo2Offset, alturaFondo2, SDL_FLIP_NONE, &fondo2x, paralaxSpeed);
	app->render->DrawTexture(fondo2, fondo2Offset + 500, alturaFondo2, SDL_FLIP_NONE, &fondo2y, paralaxSpeed);
	app->render->DrawTexture(fondo2, fondo2Offset + 1000, alturaFondo2, SDL_FLIP_NONE, &fondo2y, paralaxSpeed);
	app->render->DrawTexture(fondo2, fondo2Offset - 500, alturaFondo2, SDL_FLIP_NONE, &fondo2y, paralaxSpeed);
	app->render->DrawTexture(fondo2, fondo2Offset - 1000, alturaFondo2, SDL_FLIP_NONE, &fondo2y, paralaxSpeed);




	posJugadorParalax = (int)(player->position.x * paralaxSpeed);
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


	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->SaveRequest();
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) { 
		app->fadeToBlack->FadeToBlackTransition(app->scene, app->scene, true);
		
		//app->LoadRequest(); 
	};

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {
		//ret = false;
		app->pause = true;
		app->interface->pauseMenuOpened = true;
	
	}
		

	
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

// L14: TODO 6: Implement a method to load the state
// for now load camera's x and y
bool Scene::LoadState(pugi::xml_node node, bool loadSceneLevel) {

	//if (app->sceneLevel != node.child("player").attribute("sceneLevel").as_int()) {
	//	app->sceneLevel = node.child("player").attribute("sceneLevel").as_int();
	//	app->fadeToBlack->FadeToBlackTransition(app->scene, app->scene); //Esto no lo hace creo
	//}
	//else {
	//	app->sceneLevel = node.child("player").attribute("sceneLevel").as_int();
	//	player->position.x = node.child("player").attribute("x").as_int() + 16;
	//	player->position.y = node.child("player").attribute("y").as_int() + 16;
	//	player->SetPosition(node.child("player").attribute("x").as_int(), node.child("player").attribute("y").as_int());
	//	
	//}
	
	app->sceneLevel = node.child("player").attribute("sceneLevel").as_int();
	
	if (player != nullptr) {
		player->position.x = node.child("player").attribute("x").as_int() + 16;
		player->position.y = node.child("player").attribute("y").as_int() + 16;
		player->SetPosition(node.child("player").attribute("x").as_int(), node.child("player").attribute("y").as_int());
		player->coins = node.child("player").attribute("coins").as_int();
	}
	app->render->camera.x = node.child("camera").attribute("x").as_int() + 16;
	app->render->camera.y = node.child("camera").attribute("y").as_int() + 16;

	for (pugi::xml_node itemNode = node.child("coinsPicked").child("coin"); itemNode; itemNode = itemNode.next_sibling("coin"))
	{
		if (!itemNode.attribute("active").as_bool()) {
			app->entityManager->coins_to_destroyPos.Add(iPoint(itemNode.attribute("x").as_int(), itemNode.attribute("y").as_int()));
		}
	}
	app->entityManager->DestroyAllCoinsToDestroy();
	



	for (pugi::xml_node itemNode = node.child("enemies").child("enemy"); itemNode; itemNode = itemNode.next_sibling("enemy"))
	{
		if (!itemNode.attribute("active").as_bool()) {
			app->entityManager->enemies_to_destroyPos.Add(iPoint(itemNode.attribute("x").as_int(), itemNode.attribute("y").as_int()));
		}
	}
	app->entityManager->DestroyAllEnemiesToDestroy();

	
	return true;
}

// L14: TODO 8: Create a method to save the state of the renderer
// using append_child and append_attribute
bool Scene::SaveState(pugi::xml_node node) {

	pugi::xml_node playerNode = node.append_child("player");
	playerNode.append_attribute("x").set_value(player->position.x);
	playerNode.append_attribute("y").set_value(player->position.y);
	playerNode.append_attribute("sceneLevel").set_value(app->sceneLevel);

	playerNode.append_attribute("coins").set_value(app->scene->getPlayer()->coins);

	pugi::xml_node cameraNode = node.append_child("camera");
	cameraNode.append_attribute("x").set_value(app->render->camera.x);
	cameraNode.append_attribute("y").set_value(app->render->camera.y);
	


	pugi::xml_node coinsListNode = node.append_child("coinsPicked");
	
	for (int i = 0; i < app->entityManager->coins.Count(); i++) {
		pugi::xml_node cointNode = coinsListNode.append_child("coin");
		cointNode.append_attribute("x").set_value(app->entityManager->coins.At(i)->data->position.x);
		cointNode.append_attribute("y").set_value(app->entityManager->coins.At(i)->data->position.y);
		cointNode.append_attribute("active").set_value(app->entityManager->coins.At(i)->data->active);
	}


	pugi::xml_node enemiesListNode = node.append_child("enemies");
	

	for (int i = 0; i < app->entityManager->enemies.Count(); i++) {
		pugi::xml_node enemyNode = enemiesListNode.append_child("enemy");
		enemyNode.append_attribute("x").set_value(app->entityManager->enemies.At(i)->data->originalPosition.x);
		enemyNode.append_attribute("y").set_value(app->entityManager->enemies.At(i)->data->originalPosition.y);
		enemyNode.append_attribute("active").set_value(app->entityManager->enemies.At(i)->data->active);
	}


	return true;
}
bool Scene::OnGuiMouseClickEvent(GuiControl* control)
{
	// L15: DONE 5: Implement the OnGuiMouseClickEvent method
	LOG("Press Gui Control: %d", control->id);

	return true;
}