#ifndef __INTERFACE_H__
#define __INTERFACE_H__

#include "Module.h"
#include "Player.h"
#include "Item.h"
#include "GuiControl.h"
#include "GuiControlButton.h"

struct SDL_Texture;

class Interface : public Module
{
public:

	Interface(bool start_enabled = true);

	// Destructor
	virtual ~Interface();

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

	bool PostLateUpdate();
	// Called before quitting
	bool CleanUp();

	bool OnGuiMouseClickEvent(GuiControl* control);


	void ShowHUD();
	void ShowPauseMenu();
	void ShowPauseMenuSettings();

	void Fullscreen();

	bool LoadState(pugi::xml_node node);
	bool SaveState(pugi::xml_node node);
	
	//SDL_Texture* img;
	//SDL_Texture* fondo1;
	//SDL_Texture* fondo2;
	//SDL_Texture* fondo0;
	SDL_Texture* settingsGUI;

	bool pauseMenuOpened = false;
	bool settingsOpened = false;
	
	


private:
	
	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;


	SDL_Texture* heartHolderTex;

	SDL_Texture* heartTex;
	SDL_Texture* noHeartTex;

	SDL_Texture* bossRed;
	SDL_Texture* bossBG;

	SDL_Texture* backgroundGUI;
	SDL_Texture* normalButton;
	SDL_Texture* pressedButton;
	
	SDL_Texture* coinTexture;

	GuiControlButton* gcButtom;

	List<GuiControl*> pauseMenuButtons;
	List<GuiControl*> pauseMenuSettingsButtons;
	
	bool _pauseMenuOpened = false;
	bool _settingsOpened = false;

	Timer nivelTimer;
	
};

#endif // __Interface_H__