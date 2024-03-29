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
#include "GuiControlSlider.h"
#include "GuiControlCheckBox.h"

Interface::Interface(bool start_enabled) : Module(start_enabled)
{
	name.Create("interface");
	
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

	bossBG = app->tex->Load("Assets/UI/boss_bgXL.png");
	bossRed = app->tex->Load("Assets/UI/boss_redXL.png");

	backgroundGUI = app->tex->Load("Assets/UI/backgroundGUI.png");
	settingsGUI = app->tex->Load("Assets/UI/settingsGUI.png");

	normalButton = app->tex->Load("Assets/UI/normalButton.png");
	pressedButton = app->tex->Load("Assets/UI/pressedButton.png");

	coinTexture = app->tex->Load("Assets/UI/coin.png");

	dieScreen = app->tex->Load("Assets/UI/dieScreen.png");
	winScreen = app->tex->Load("Assets/UI/winScreen.png");

	showWinScreen = false;
	showDieScreen = false;

	nivelTimer.Start();

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

	if (app->scene->active) {
		ShowHUD();





		//Boss life
		if (app->entityManager->boss != nullptr) {
			if (app->entityManager->boss->activeBoss) {
				
				SDL_Rect rect = { 0,0, 512, 48 };
				int rectW = rect.w;
				app->render->DrawTexture(bossBG, windowW/2-(rectW/2), 50, SDL_FLIP_NONE, 0, 0, 0, INT_MAX, INT_MAX, 1);
				rect.w = (rect.w * app->entityManager->boss->health) / app->entityManager->boss->maxHealth;
				app->render->DrawTexture(bossRed, windowW / 2 - (rectW / 2), 50, SDL_FLIP_NONE, &rect, 0, 0, INT_MAX, INT_MAX, 1);
			}
		}






		ShowPauseMenu();
	}

	if (showDieScreen) {
		app->render->DrawTexture(dieScreen, 0, 0, 0);
	}

	if (showWinScreen) {
		app->render->DrawTexture(winScreen, 0, 0, 0);
	}
	
	
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

	ListItem<GuiControl*>* controlListItem = nullptr;
	for (controlListItem = pauseMenuButtons.start; controlListItem != NULL; controlListItem = controlListItem->next)
	{
		app->guiManager->DestroyGuiControl(controlListItem->data);
	}
	pauseMenuButtons.Clear();

	return true;
}




void Interface::ShowHUD()
{

	app->render->DrawTexture(heartHolderTex, 2, 2, 0);


	
	int tama�oNumero;
	if (app->scene->getPlayer()->coins < 10) {
		tama�oNumero = 15;
	}
	else if (app->scene->getPlayer()->coins < 100) {
		tama�oNumero = 30;
	}
	else {
		tama�oNumero = 45;
	}

	app->render->DrawText(std::to_string(app->scene->getPlayer()->coins).c_str(), 60, 60, tama�oNumero, 23);


	int segundos = nivelTimer.ReadSec() % 60;
	int minutos = nivelTimer.ReadSec() / 60;
	char tiempo[10];
	sprintf_s(tiempo, "%02d:%02d", minutos, segundos);

	app->render->DrawText(tiempo, windowW - 130, 30, 100, 35);


	//coinTexture
	app->render->DrawTexture(coinTexture, 10, 30, 0);


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
		
		
		pauseMenuButtons.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 101, "CONTINUE", SDL_Rect{ (int)windowW / 2 - 68,	(int)windowH / 2 - 30,	136,46 }, this));
		pauseMenuButtons.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 102, "SETTINGS", SDL_Rect{ (int)windowW / 2 - 68,	(int)windowH / 2 + 40,	136,46 }, this));
		pauseMenuButtons.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 103, "MAIN MENU", SDL_Rect{ (int)windowW / 2 - 68,	(int)windowH / 2 + 110,	136,46 }, this));
		pauseMenuButtons.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 104, "EXIT GAME", SDL_Rect{ (int)windowW / 2 - 68,	(int)windowH / 2 + 180,	136,46 }, this));
	
		_pauseMenuOpened = true;
	}
	if (pauseMenuOpened) {
		//Fondo etc...
		app->render->DrawTexture(backgroundGUI, 0, 0, 0);
		ShowPauseMenuSettings();
	}

	
	
	

}

void Interface::ShowPauseMenuSettings()
{
	if (settingsOpened && !_settingsOpened) {

		pauseMenuSettingsButtons.Add(app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 1011, "", SDL_Rect{ (int)windowW / 2 + 60,	(int)windowH / 2 - 10,	120,20 }, this));
		((GuiControlSlider*)(pauseMenuSettingsButtons.At(pauseMenuSettingsButtons.Count() - 1)->data))->value = app->audio->musicVolumne;

		pauseMenuSettingsButtons.Add(app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 1021, "", SDL_Rect{ (int)windowW / 2 + 60,	(int)windowH / 2 + 50,	120,20 }, this));
		((GuiControlSlider*)(pauseMenuSettingsButtons.At(pauseMenuSettingsButtons.Count() - 1)->data))->value = app->audio->sfvVolumne;

		pauseMenuSettingsButtons.Add(app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 1031, "", SDL_Rect{ (int)windowW / 2 - 110,	(int)windowH / 2 + 180,	20,20 }, this));

		if (app->fullscreen)
		{
			((GuiControlCheckBox*)(pauseMenuSettingsButtons.At(pauseMenuSettingsButtons.Count() - 1)->data))->isChecked = true;
		}
		else
		{
			((GuiControlCheckBox*)(pauseMenuSettingsButtons.At(pauseMenuSettingsButtons.Count() - 1)->data))->isChecked = false;
		}

		pauseMenuSettingsButtons.Add(app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 1041, "", SDL_Rect{ (int)windowW / 2 + 80,	(int)windowH / 2 + 180,	20,20 }, this));

		if (app->render->vsync)
		{
			((GuiControlCheckBox*)(pauseMenuSettingsButtons.At(pauseMenuSettingsButtons.Count() - 1)->data))->isChecked = true;
		}
		else
		{
			((GuiControlCheckBox*)(pauseMenuSettingsButtons.At(pauseMenuSettingsButtons.Count() - 1)->data))->isChecked = false;
		}

		pauseMenuSettingsButtons.Add(app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1005, "Return", SDL_Rect{ (int)windowW / 2 - 68,	(int)windowH - 150,	136,46 }, this));

		_settingsOpened = true;
	}
	if (settingsOpened) {
		app->render->DrawTexture(settingsGUI, 0, 0, 0);
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
			app->pause = false;


			break;
		case 102:
			settingsOpened = true;
			
			for (controlListItem = pauseMenuButtons.start; controlListItem != NULL; controlListItem = controlListItem->next)
			{
				controlListItem->data->state = GuiControlState::DISABLED;
			}

			break;
		case 103:
			_pauseMenuOpened = false;
			pauseMenuOpened = false;

			controlListItem = nullptr;
			for (controlListItem = pauseMenuButtons.start; controlListItem != NULL; controlListItem = controlListItem->next)
			{
				app->guiManager->DestroyGuiControl(controlListItem->data);
			}
			pauseMenuButtons.Clear();
			app->pause = false;
			app->fadeToBlack->FadeToBlackTransition(app->scene, app->sceneMainMenu);

			break;
		case 104:
			app->closeApplication = true;
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

		case 1011:
			

			app->audio->musicVolumne = ((GuiControlSlider*)control)->value;

			break;

		case 1021:
			app->audio->sfvVolumne = ((GuiControlSlider*)control)->value;
			break;

		case 1031:
			Fullscreen();
			break;

		case 1041:
			if (app->render->vsync)
			{
				app->render->vsync = false; 
			}
			else
			{
				app->render->vsync = true;
			}
			break;

		
	default:
		break;
	}

	return true;
}

void Interface::Fullscreen() {
	//if (app->fullscreen) {
	//	SDL_SetWindowFullscreen(app->win->window, 0);  // Modo de ventana
	//}
	//else {
	//	SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_FULLSCREEN_DESKTOP);  // Pantalla completa
	//}
	//app->fullscreen = !app->fullscreen;

	//int newWindowWidth, newWindowHeight;
	//SDL_GetWindowSize(app->win->window, &newWindowWidth, &newWindowHeight);

	//// Cambiar el tama�o del renderizador para que coincida con la nueva resoluci�n
	//SDL_RenderSetLogicalSize(app->render->renderer, newWindowWidth, newWindowHeight);

	////// Actualizar el tama�o de la ventana
	////windowWidth = newWindowWidth;
	////windowHeight = newWindowHeight;


	//Uint32 fullscreenFlag = SDL_WINDOW_FULLSCREEN_DESKTOP;
	//bool isFullscreen = SDL_GetWindowFlags(app->win->window) & fullscreenFlag;

	//SDL_SetWindowFullscreen(app->win->window, isFullscreen ? 0 : fullscreenFlag);
	//int ww, wh;
	//SDL_GetWindowSize(app->win->window, &ww, &wh);

	//SDL_SetRenderDrawColor(app->render->renderer, 0, 0, 0, 255);

	//if ((float)ww / wh != (float)512 / 384) {
	//	// Calcular el tama�o de los bordes
	//	int borderWidth = (wh * 512 / 384 - ww) / 2;

	//	// Dibujar los bordes izquierdo y derecho
	//	SDL_Rect rect1 = { 0, 0, borderWidth, wh };
	//	SDL_Rect rect2 = {ww - borderWidth, 0, borderWidth, wh};
	//	SDL_RenderFillRect(app->render->renderer, &rect1);
	//	SDL_RenderFillRect(app->render->renderer, &rect2);
	//}

	// Obtener el tama�o de la pantalla


	SDL_DisplayMode modoPantalla;
	SDL_GetCurrentDisplayMode(0, &modoPantalla);

	// Configurar la ventana para ser pantalla completa sin bordes
	

	// Establecer el tama�o de la ventana para que coincida con el tama�o de la pantalla

	if (!app->fullscreen)
	{	
		SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_FULLSCREEN);
		SDL_SetWindowBordered(app->win->window, SDL_FALSE);

		SDL_SetWindowSize(app->win->window, modoPantalla.w, modoPantalla.h);
		
		app->fullscreen = true;
	}
	else
	{
		SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_FOREIGN);
		SDL_SetWindowBordered(app->win->window, SDL_TRUE);

		SDL_SetWindowSize(app->win->window, 1024, 768);

		app->fullscreen = false;
	}
}