#include "GuiControlButton.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "Log.h"
#include "Window.h"
#include "Textures.h"

GuiControlButton::GuiControlButton(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;

	canClick = true;
	drawBasic = false;

	normalButton = app->tex->Load("Assets/UI/normalButton.png");
	pressedButton = app->tex->Load("Assets/UI/pressedButton.png");
	hoverButton = app->tex->Load("Assets/UI/focusedButton.png");

	selectButton = app->audio->LoadFx("Assets/Audio/Fx/selectButton.wav");
	clickButton = app->audio->LoadFx("Assets/Audio/Fx/clickButton.wav");
}

GuiControlButton::~GuiControlButton()
{
	app->tex->UnLoad(normalButton);
	app->tex->UnLoad(pressedButton);
}

bool GuiControlButton::Start() {
	

	if (normalButton == nullptr || pressedButton == nullptr)
	{
		LOG("Error cargando texturas para botones");
		return false;
	}

	return true;
}

bool GuiControlButton::Update(float dt)
{
	
	//if (state != GuiControlState::DISABLED)
	//{
	//	// L15: DONE 3: Update the state of the GUiButton according to the mouse position
	//	app->input->GetMousePosition(mouseX, mouseY);

	//	//If the position of the mouse if inside the bounds of the button 
	//	if (mouseX > bounds.x && mouseX < bounds.x + bounds.w && mouseY > bounds.y && mouseY < bounds.y + bounds.h) {
	//	
	//		state = GuiControlState::FOCUSED;

	//		if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
	//			state = GuiControlState::PRESSED;
	//		}
	//		
	//		if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {
	//			NotifyObserver();
	//		}
	//	}
	//	else {
	//		state = GuiControlState::NORMAL;
	//	}

	//	//L15: DONE 4: Draw the button according the GuiControl State
	//	switch (state)
	//	{
	//	case GuiControlState::DISABLED:
	//		app->render->DrawRectangle(bounds, 200, 200, 200, 255, true, false);
	//		break;
	//	case GuiControlState::NORMAL:
	//		app->render->DrawRectangle(bounds, 0, 0, 255, 255, true, false);
	//		break;
	//	case GuiControlState::FOCUSED:
	//		app->render->DrawRectangle(bounds, 0, 0, 20, 255, true, false);
	//		break;
	//	case GuiControlState::PRESSED:
	//		app->render->DrawRectangle(bounds, 0, 255, 0, 255, true, false);
	//		break;
	//	}

	//	app->render->DrawText(text.GetString(), bounds.x, bounds.y, bounds.w, bounds.h);

	//}

	return false;
}

bool GuiControlButton::PostUpdate()
{
	if (state != GuiControlState::DISABLED)
	{
		// L15: DONE 3: Update the state of the GUiButton according to the mouse position
		app->input->GetMousePosition(mouseX, mouseY);
		
		mouseX *= app->win->GetScale();
		mouseY *= app->win->GetScale();

		//If the position of the mouse if inside the bounds of the button 
		if (mouseX > bounds.x && mouseX < bounds.x + bounds.w && mouseY > bounds.y && mouseY < bounds.y + bounds.h) {

			state = GuiControlState::FOCUSED;

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
				state = GuiControlState::PRESSED;
			}

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {
				NotifyObserver();
			}
		}
		else {
			state = GuiControlState::NORMAL;
		}

		//L15: DONE 4: Draw the button according the GuiControl State
		switch (state)
		{
		case GuiControlState::DISABLED:
			app->render->DrawTexture(normalButton, bounds.x , bounds.y, 0);
			//app->render->DrawRectangle(bounds, 0, 0, 255, 255, true, false);
			break;
		case GuiControlState::NORMAL:
			sonido = true;
			sonido2 = true;
			app->render->DrawTexture(normalButton, bounds.x / app->win->GetScale(), bounds.y / app->win->GetScale(),0);
			//app->render->DrawRectangle(bounds, 0, 0, 255, 255, true, false);
			break;
		case GuiControlState::FOCUSED:
			if (sonido)
			{
				app->audio->PlayFx(selectButton);
				sonido = false;
			}
			
			app->render->DrawTexture(hoverButton, bounds.x / app->win->GetScale(), bounds.y / app->win->GetScale(),0);
			//app->render->DrawRectangle(bounds, 0, 0, 255, 100, true, false);
			break;
		case GuiControlState::PRESSED:
			//app->render->DrawRectangle(bounds, 0, 255, 0, 255, true, false);
			if (sonido2)
			{
				app->audio->PlayFx(clickButton);
				sonido2 = false;
			}
			
			app->render->DrawTexture(pressedButton, bounds.x / app->win->GetScale(), bounds.y / app->win->GetScale(), 0);
			break;
		}

		app->render->DrawText(text.GetString(), bounds.x + 20, bounds.y + 10, bounds.w - 40, bounds.h - 20,{128, 116, 100});

	}
	return false;
}

