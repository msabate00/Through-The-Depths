#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "SceneMainMenu.h"
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

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	
	return ret;
}

// Called before quitting
bool SceneMainMenu::CleanUp()
{
	LOG("Freeing SceneMainMenu");

	return true;
}


bool SceneMainMenu::LoadState(pugi::xml_node node) {

	

	
	return true;
}
bool SceneMainMenu::SaveState(pugi::xml_node node) {

	


	return true;
}
bool SceneMainMenu::OnGuiMouseClickEvent(GuiControl* control)
{
	
	LOG("Press Gui Control: %d", control->id);

	return true;
}