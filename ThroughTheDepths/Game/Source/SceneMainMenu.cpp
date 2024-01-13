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


	SDL_Rect btPos = { windowW / 2 - 60, windowH / 2 - 50, 120,20 };
	gcButtom = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "START", btPos, this);
	controlsScene.Add(gcButtom);

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
			app->fadeToBlack->FadeToBlackTransition(app->sceneMainMenu, app->scene);
			break;

		default:
			break;
	}

	return true;
}