#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Interface.h"
#include "Map.h"
#include "Chest.h"
#include "FadeToBlack.h"
#include "GuiManager.h"

#include "Defs.h"
#include "Log.h"

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

	//configInterface = &config;
	
	

	// iterate all objects in the Interface
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
bool Interface::Start()
{
	

	SDL_Rect btPos = { windowW / 2 - 60, windowH / 2 - 10, 120,20 };
	gcButtom = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "MyButton", btPos, this);



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