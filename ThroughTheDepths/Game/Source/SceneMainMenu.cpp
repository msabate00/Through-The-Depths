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
	name.Create("settings");

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
	creditsGUI = app->tex->Load("Assets/UI/creditsGUI.png");
	backgroundGUI = app->tex->Load("Assets/UI/backgroundGUI.png");
	settingsGUI = app->tex->Load("Assets/UI/settingsGUI.png");

	// NOTE: We have to avoid the use of paths in the code, we will move it later to a config file
	//img = app->tex->Load("Assets/Textures/test.png");

	//Music is commented so that you can add your own music
	app->audio->PlayMusic("Assets/Audio/Music/menu.ogg");

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	//Get the size of the texture
	//app->tex->GetSize(creditsGUI, windowW, windowH);
	//app->tex->GetSize(backgroundGUI, windowW, windowH);
	//app->tex->GetSize(settingsGUI, windowW, windowH);


	/*SDL_Rect btPos = { windowW / 2 - 60, windowH / 2 - 10, 120,20 };
	gcButtom = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "MyButton", btPos, this);*/

	controlsScene.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "NEW GAME",	SDL_Rect{ (int)windowW / 2 - 68,	(int)windowH / 2 - 30,	136,46 }, this));
	controlsScene.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "CONTINUE",	SDL_Rect{ (int)windowW / 2 - 68,	(int)windowH / 2 + 40,	136,46 }, this));
	controlsScene.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "SETTINGS",	SDL_Rect{ (int)windowW / 2 - 68,	(int)windowH / 2 + 110,	136,46 }, this));
	controlsScene.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, "CREDITS",	SDL_Rect{ (int)windowW / 2 - 68,	(int)windowH / 2 + 180,	136,46 }, this));
	controlsScene.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, "EXIT",		SDL_Rect{ (int)windowW / 2 - 68,	(int)windowH / 2 + 250,	136,46 }, this));

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
	
	if (showSettings && !_showSettings) {

		SettingsInterface();
		
	}
	if (showSettings) { app->render->DrawTexture(settingsGUI, 0, 0, 0); }
	if (showCredits) {
		ShowCredits();
	}


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
	

	ListItem<GuiControl*>* control;
	for (control = controlsScene.start; control != NULL; control = control->next)
	{
		app->guiManager->DestroyGuiControl(control->data);
	}

	

	return true;
}


bool SceneMainMenu::OnGuiMouseClickEvent(GuiControl* control)
{
	// L15: DONE 5: Implement the OnGuiMouseClickEvent method
	LOG("Press Gui Control: %d", control->id);

	switch (control->id)
	{

		case 1:
			app->fadeToBlack->newgame = true;
			app->sceneLevel = 0;
			app->fadeToBlack->FadeToBlackTransition(app->sceneMainMenu, app->scene, false, 60, true);
			break;

		case 2:
			app->LoadRequest();
			app->fadeToBlack->newgame = false;
			app->fadeToBlack->FadeToBlackTransition(app->sceneMainMenu, app->scene, true, 60, true, false);
			break;

		case 3:
			showSettings = true;
			break;

		case 4:
			showCredits = true;
			break;
		
		case 5:
			app->closeApplication = true;
			break;


		case 105:

			showSettings = false;
			_showSettings = false;
			DestroySettingsInterface();
			break;


		case 106:
			showCredits = false;
			_showCredits = false;
			ListItem<GuiControl*>* control;
			for (control = controlsScene.start; control != NULL; control = control->next)
			{
				control->data->state = GuiControlState::NORMAL;
			}
			app->guiManager->DestroyGuiControl(gcCloseCredits);
			break;

		default:
			break;
	}

	return true;
}

void SceneMainMenu::SettingsInterface()
{
	

	ListItem<GuiControl*>* control;
	for (control = controlsScene.start; control != NULL; control = control->next)
	{
		control->data->state = GuiControlState::DISABLED;
	}


	//MENU AJUSTES

	controlsSettings.Add(app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 1011, "", SDL_Rect{ (int)windowW / 2 + 60,	(int)windowH / 2 -10,	120,20 }, this));
	controlsSettings.Add(app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 1021, "", SDL_Rect{ (int)windowW / 2 + 60,	(int)windowH / 2 +50,	120,20 }, this));
	controlsSettings.Add(app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 1031, "", SDL_Rect{ (int)windowW / 2 - 110,	(int)windowH / 2 +180,	20,20 }, this));
	controlsSettings.Add(app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 1041, "", SDL_Rect{ (int)windowW / 2 + 80,	(int)windowH / 2 + 180,	20,20 }, this));
	controlsSettings.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 105, "Return", SDL_Rect{ (int)windowW / 2 - 68,	(int)windowH - 150,	136,46 }, this));
	_showCredits = true;
	_showSettings = true;

	
}

void SceneMainMenu::ShowCredits()
{
	if (showCredits && !_showCredits) {
		ListItem<GuiControl*>* control;
		for (control = controlsScene.start; control != NULL; control = control->next)
		{
			control->data->state = GuiControlState::DISABLED;
		}

		gcCloseCredits = app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 106, "Return", SDL_Rect{ (int)windowW / 2 - 68,	(int)windowH  - 150,	136,46 }, this);
		_showCredits = true;
	}

	app->render->DrawTexture(creditsGUI, 0, 0, 0);

	//Mostrar imagen o lo que sea

}

void SceneMainMenu::DestroySettingsInterface()
{
	ListItem<GuiControl*>* control;
	for (control = controlsSettings.start; control != NULL; control = control->next)
	{
		app->guiManager->DestroyGuiControl(control->data);
	}
	controlsSettings.Clear();


	
	for (control = controlsScene.start; control != NULL; control = control->next)
	{
		control->data->state = GuiControlState::NORMAL;
	}

}

bool SceneMainMenu::LoadState(pugi::xml_node node)
{

	return false;
}

bool SceneMainMenu::SaveState(pugi::xml_node node)
{

	




	return false;
}
