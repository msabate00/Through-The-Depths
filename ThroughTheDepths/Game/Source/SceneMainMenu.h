#ifndef __MAINMENU_H__
#define __MAINMENU_H__

#include "Module.h"
#include "Player.h"
#include "Item.h"
#include "GuiControl.h"
#include "GuiControlButton.h"

struct SDL_Texture;

class SceneMainMenu : public Module
{
public:

	SceneMainMenu(bool start_enabled = true);

	// Destructor
	virtual ~SceneMainMenu();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool OnGuiMouseClickEvent(GuiControl* control);

	void SettingsInterface();
	void ShowSettingsInterface();
	void ShowCredits();
	void DestroySettingsInterface();

	bool LoadState(pugi::xml_node node);
	bool SaveState(pugi::xml_node node);
	
	SDL_Texture* img;
	SDL_Texture* fondo1;
	SDL_Texture* fondo2;
	SDL_Texture* fondo0;
	SDL_Texture* backgroundGUI;
	SDL_Texture* normalButton;
	SDL_Texture* pressedButton;
	SDL_Texture* creditsGUI;
	SDL_Texture* settingsGUI;
	SDL_Texture* backgroundSettings;
	SDL_Texture* backgroundMenu;

	SDL_Texture* introImage;

private:
	
	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;
	Player* player;


	int fondo0Offset = 0;
	int fondo1Offset = 0;
	int fondo2Offset = 0;

	
	List<GuiControl*> controlsScene;
	List<GuiControl*> controlsSettings;
	GuiControlButton* gcButtom;
	GuiControl* gcCloseCredits;
	
	bool showSettings = false;
	bool _showSettings = false;

	bool showCredits = false;
	bool _showCredits = false;

	SDL_Texture* hearthHolderTex;

	Timer timerIntro;

	
};

#endif // __SceneMainMenuMAINMENUE_H__