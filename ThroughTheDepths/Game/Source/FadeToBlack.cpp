#include "FadeToBlack.h"

#include "App.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Scene.h"
#include "Physics.h"
#include "Interface.h"

#include "SDL/include/SDL_render.h"
#include "Log.h"


FadeToBlack::FadeToBlack(bool startEnabled) : Module(startEnabled)
{


}

FadeToBlack::~FadeToBlack()
{

}

bool FadeToBlack::Start()
{
	LOG("Preparing Fade Screen");

	uint windowW, windowH;

	app->win->GetWindowSize(windowW, windowH);
	screenRect = { 0, 0, (int)(windowW * app->win->GetScale()),  (int)(windowH * app->win->GetScale()) };

	currentStep = Fade_Step::NONE;

	// Enable blending mode for transparency
	SDL_SetRenderDrawBlendMode(app->render->renderer, SDL_BLENDMODE_BLEND);
	return true;
}

bool FadeToBlack::Update(float dt)
{
	
	if (currentStep == Fade_Step::NONE) return true;

	if (currentStep == Fade_Step::TO_BLACK)
	{
		
		++frameCount;
		if (frameCount >= maxFadeFrames)
		{
			// TODO 1: Enable / Disable the modules received when FadeToBlacks(...) gets called
			app->map->Disable();
			moduleToDisable->Disable();
			app->entityManager->Disable();
			app->interface->Disable();
			
			if (reloadScene) {
				if (!newgame) {
					app->LoadRequest();
				}
				reloadScene = false;
			}
			
			if (!app->loadRequest) {
				app->map->Enable();
				moduleToEnable->Enable();
				app->entityManager->Enable();
				app->interface->Enable();
				currentStep = Fade_Step::FROM_BLACK;
				if (!newgame) {
					app->LoadRequest();
				}
				
			}

			
		}
		
	}
	else
	{
		--frameCount;
		if (frameCount <= 0)
		{
			
			currentStep = Fade_Step::NONE;

			
			//if (reloadScene) {
			//	//app->LoadRequest();
			//	reloadScene = false;
			//	FadeToBlackTransition(app->scene, app->scene);
			//}
			
		}
	}

	return true;
}

bool FadeToBlack::PostUpdate()
{
	// Exit this function if we are not performing a fade
	if (currentStep == Fade_Step::NONE) return true;

	float fadeRatio = (float)frameCount / (float)maxFadeFrames;

	// Render the black square with alpha on the screen
	SDL_SetRenderDrawColor(app->render->renderer, 0, 0, 0, (Uint8)(fadeRatio * 255.0f));
	SDL_RenderFillRect(app->render->renderer, &screenRect);

	return true;
}

bool FadeToBlack::FadeToBlackTransition(Module* moduleToDisable, Module* moduleToEnable, bool load,float frames, bool force)
{
	bool ret = false;

	// If we are already in a fade process, ignore this call
	if (currentStep == Fade_Step::NONE || force)
	{
		reloadScene = load;
		currentStep = Fade_Step::TO_BLACK;
		frameCount = 0;
		maxFadeFrames = frames;

		// TODO 1: How do we keep track of the modules received in this function?
		this->moduleToDisable = moduleToDisable;
		this->moduleToEnable = moduleToEnable;

		ret = true;
	}

	return ret;
}