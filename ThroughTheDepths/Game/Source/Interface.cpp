#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Interface.h"
#include "GuiManager.h"
#include "Scene.h"
#include "FadeToBlack.h"
#include "SceneMainMenu.h"

#include "Defs.h"
#include "Log.h"

Interface::Interface(bool start_enabled) : Module(start_enabled)
{
	name.Create("settings");
	
}

// Destructor
Interface::~Interface()
{}

// Called before render is available
bool Interface::Awake(pugi::xml_node& config)
{
	LOG("Loading Interface");
	bool ret = true;

	

	
	

	return ret;
}

// Called before the first frame
bool Interface::Start()
{
	

	app->win->GetWindowSize(windowW, windowH);
	//SDL_Rect btPos = { windowW / 2 - 60, windowH / 2 - 10, 120,20 };
	//gcButtom = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "MyButton", btPos, this);


	/*btPos.y -= 100;
	gcButtom = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::IMAGE, 1, "Assets/UI/Free Paper UI System/1 Sprites/Content/5 Holders/1.png", btPos, this);*/

	heartTex = app->tex->Load("Assets/UI/Free Paper UI System/1 Sprites/Content/6 High lighter/7.png");
	noHeartTex = app->tex->Load("Assets/UI/Free Paper UI System/1 Sprites/Content/6 High lighter/6.png");
	heartHolderTex = app->tex->Load("Assets/UI/Free Paper UI System/1 Sprites/Content/4 Buttons/hearthHolder.png");

	return true;
}

// Called each loop iteration
bool Interface::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Interface::Update(float dt)
{


	return true;
}

// Called each loop iteration
bool Interface::PostUpdate()
{
	bool ret = true;

	
	ShowHUD();
	
	ShowPauseMenu();


	
	
	return ret;
}

// Called each loop iteration
bool Interface::PostLateUpdate()
{
	bool ret = true;


	return ret;
}

// Called before quitting
bool Interface::CleanUp()
{
	LOG("Freeing Interface");

	return true;
}




void Interface::ShowHUD()
{

	app->render->DrawTexture(heartHolderTex, 2, 2, 0);

	if (app->scene->getPlayer()->health >= 1) {
		app->render->DrawTexture(heartTex, 15, 10, 0);
	}
	else {
		app->render->DrawTexture(noHeartTex, 15, 10, 0);
	}

	if (app->scene->getPlayer()->health >= 2) {
		app->render->DrawTexture(heartTex, 25, 10, 0);
	}
	else {
		app->render->DrawTexture(noHeartTex, 25, 10, 0);
	}

	if (app->scene->getPlayer()->health >= 3) {
		app->render->DrawTexture(heartTex, 35, 10, 0);
	}
	else {
		app->render->DrawTexture(noHeartTex, 35, 10, 0);
	}

	if (app->scene->getPlayer()->health >= 4) {
		app->render->DrawTexture(heartTex, 45, 10, 0);
	}
	else {
		app->render->DrawTexture(noHeartTex, 45, 10, 0);
	}

	if (app->scene->getPlayer()->health >= 5) {
		app->render->DrawTexture(heartTex, 55, 10, 0);
	}
	else {
		app->render->DrawTexture(noHeartTex, 55, 10, 0);
	}
}

void Interface::ShowPauseMenu()
{
	if (pauseMenuOpened && !_pauseMenuOpened) {

		
		pauseMenuButtons.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 101, "CONTINUE", SDL_Rect{ (int)windowW / 2 - 60,	(int)windowH / 2 - 60,	120,20 }, this));
		pauseMenuButtons.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 102, "SETTINGS", SDL_Rect{ (int)windowW / 2 - 60,	(int)windowH / 2 - 30,	120,20 }, this));
		pauseMenuButtons.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 103, "RETURN TO MAIN MENU", SDL_Rect{ (int)windowW / 2 - 60,	(int)windowH / 2,	120,20 }, this));
		pauseMenuButtons.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 104, "EXIT GAME", SDL_Rect{ (int)windowW / 2 - 60,	(int)windowH / 2 + 30,	120,20 }, this));
	
		_pauseMenuOpened = true;
	}
	if (pauseMenuOpened) {
		//Fondo etc...

		ShowPauseMenuSettings();
	}

	
	
	

}

void Interface::ShowPauseMenuSettings()
{

	if (settingsOpened && !_settingsOpened) {

		pauseMenuSettingsButtons.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 101, "Musica", SDL_Rect{ (int)windowW / 2 - 180,	(int)windowH / 2 - 70,	120,20 }, this));
		pauseMenuSettingsButtons.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1011, "Musica", SDL_Rect{ (int)windowW / 2 + 60,	(int)windowH / 2 - 70,	120,20 }, this));
		pauseMenuSettingsButtons.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 102, "Fx", SDL_Rect{ (int)windowW / 2 - 180,	(int)windowH / 2 - 40,	120,20 }, this));
		pauseMenuSettingsButtons.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1021, "Fx", SDL_Rect{ (int)windowW / 2 + 60,	(int)windowH / 2 - 40,	120,20 }, this));
		pauseMenuSettingsButtons.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 103, "Fullscreen", SDL_Rect{ (int)windowW / 2 - 180,	(int)windowH / 2 - 10,	120,20 }, this));
		pauseMenuSettingsButtons.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1031, "Fullscreen", SDL_Rect{ (int)windowW / 2 + 60,	(int)windowH / 2 - 10,	120,20 }, this));
		pauseMenuSettingsButtons.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 104, "Vsync", SDL_Rect{ (int)windowW / 2 - 180,	(int)windowH / 2 + 20,	120,20 }, this));
		pauseMenuSettingsButtons.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1041, "Vsync", SDL_Rect{ (int)windowW / 2 + 60,	(int)windowH / 2 + 20,	120,20 }, this));
		pauseMenuSettingsButtons.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1005, "Return", SDL_Rect{ (int)windowW / 2 - 60,	(int)windowH / 2 + 50,	120,20 }, this));

		_settingsOpened = true;
	}
	if (settingsOpened) {
		//Fondo etc...
	}

}



bool Interface::LoadState(pugi::xml_node node)
{

	return false;
}

bool Interface::SaveState(pugi::xml_node node)
{
	LOG("GUARDADO INTERFAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAZ");
	return false;
}


bool Interface::OnGuiMouseClickEvent(GuiControl* control)
{
	// L15: DONE 5: Implement the OnGuiMouseClickEvent method
	LOG("Press Gui Control: %d", control->id);


	ListItem<GuiControl*>* controlListItem = nullptr;

	switch (control->id)
	{
		//MENU PAUSA ---------------------------------------------------------------------
		case 101:
			_pauseMenuOpened = false;
			pauseMenuOpened = false;

			controlListItem = nullptr;
			for (controlListItem = pauseMenuButtons.start; controlListItem != NULL; controlListItem = controlListItem->next)
			{
				app->guiManager->DestroyGuiControl(controlListItem->data);
			}
			pauseMenuButtons.Clear();



			break;
		case 102:

			settingsOpened = true;
			
			for (controlListItem = pauseMenuButtons.start; controlListItem != NULL; controlListItem = controlListItem->next)
			{
				controlListItem->data->state = GuiControlState::DISABLED;
			}

			break;
		case 103:

			app->fadeToBlack->FadeToBlackTransition(app->scene, app->sceneMainMenu);

			break;
		case 104:
			break;




		//MENU AJUSTES ---------------------------------------------------------------------
		case 1005:
			_settingsOpened = false;
			settingsOpened = false;

			
			for (controlListItem = pauseMenuSettingsButtons.start; controlListItem != NULL; controlListItem = controlListItem->next)
			{
				app->guiManager->DestroyGuiControl(controlListItem->data);
			}
			pauseMenuSettingsButtons.Clear();


			for (controlListItem = pauseMenuButtons.start; controlListItem != NULL; controlListItem = controlListItem->next)
			{
				controlListItem->data->state = GuiControlState::NORMAL;
			}

			break;

	default:
		break;
	}

	return true;
}