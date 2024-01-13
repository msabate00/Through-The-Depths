#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneMainMenu.h"
#include "Scene.h"
#include "Map.h"
#include "Chest.h"
#include "FadeToBlack.h"
#include "GuiManager.h"

#include "Defs.h"
#include "Log.h"

SceneMainMenu::SceneMainMenu(bool start_enabled) : Module(start_enabled)
{
	name.Create("SceneMainMenu");

}

// Destructor
SceneMainMenu::~SceneMainMenu()
{}

// Called before render is available
bool SceneMainMenu::Awake(pugi::xml_node& config)
{
	LOG("Loading SceneMainMenu");
	bool ret = true;




	return ret;
}

// Called before the first frame
bool SceneMainMenu::Start()
{

	// NOTE: We have to avoid the use of paths in the code, we will move it later to a config file
	//img = app->tex->Load("Assets/Textures/test.png");

	//Music is commented so that you can add your own music
	//app->audio->PlayMusic("Assets/Audio/Music/music_spy.ogg");

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	//Get the size of the texture
	app->tex->GetSize(img, texW, texH);


	/*SDL_Rect btPos = { windowW / 2 - 60, windowH / 2 - 10, 120,20 };
	gcButtom = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "MyButton", btPos, this);*/



	return true;
}

// Called each loop iteration
bool SceneMainMenu::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneMainMenu::Update(float dt)
{
	

	return true;
}

// Called each loop iteration
bool SceneMainMenu::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;


	return ret;
}

// Called before quitting
bool SceneMainMenu::CleanUp()
{
	LOG("Freeing SceneMainMenu");

	return true;
}

Player* SceneMainMenu::getPlayer()
{
	return player;
}

void SceneMainMenu::setPlayer(Player* new_player)
{
	player = new_player;

}

// L14: TODO 6: Implement a method to load the state
// for now load camera's x and y
bool SceneMainMenu::LoadState(pugi::xml_node node) {

	if (app->sceneLevel != node.child("player").attribute("sceneLevel").as_int()) {
		app->sceneLevel = node.child("player").attribute("sceneLevel").as_int();
		app->fadeToBlack->FadeToBlackTransition(app->scene, app->scene);
	}
	else {
		player->position.x = node.child("player").attribute("x").as_int() + 16;
		player->position.y = node.child("player").attribute("y").as_int() + 16;
		player->SetPosition(node.child("player").attribute("x").as_int(), node.child("player").attribute("y").as_int());
		app->sceneLevel = node.child("player").attribute("sceneLevel").as_int();
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
bool SceneMainMenu::SaveState(pugi::xml_node node) {

	pugi::xml_node playerNode = node.append_child("player");
	playerNode.append_attribute("x").set_value(player->position.x);
	playerNode.append_attribute("y").set_value(player->position.y);
	playerNode.append_attribute("sceneLevel").set_value(app->sceneLevel);

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
bool SceneMainMenu::OnGuiMouseClickEvent(GuiControl* control)
{
	// L15: DONE 5: Implement the OnGuiMouseClickEvent method
	LOG("Press Gui Control: %d", control->id);

	return true;
}