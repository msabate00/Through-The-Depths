#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Interface.h"
#include "GuiManager.h"
#include "Scene.h"

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
	SDL_Rect btPos = { windowW / 2 - 60, windowH / 2 - 10, 120,20 };
	gcButtom = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "MyButton", btPos, this);


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


bool Interface::OnGuiMouseClickEvent(GuiControl* control)
{
	// L15: DONE 5: Implement the OnGuiMouseClickEvent method
	LOG("Press Gui Control: %d", control->id);

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


}

void Interface::ShowPauseMenuSettings()
{


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