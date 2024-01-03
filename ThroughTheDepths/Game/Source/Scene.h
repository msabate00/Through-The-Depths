#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "Item.h"
#include "GuiControl.h"
#include "GuiControlButton.h"

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene(bool start_enabled = true);

	// Destructor
	virtual ~Scene();

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

	Player* getPlayer();
	void setPlayer(Player* new_player);

	bool LoadState(pugi::xml_node node);
	bool SaveState(pugi::xml_node node);

	bool OnGuiMouseClickEvent(GuiControl* control);


	
	SDL_Texture* img;
	SDL_Texture* fondo1;
	SDL_Texture* fondo2;
	SDL_Texture* fondo0;
private:
	
	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;
	Player* player;


	int fondo0Offset = 0;
	int fondo1Offset = 0;
	int fondo2Offset = 0;

	GuiControlButton* gcButtom;
	

	
};

#endif // __SCENE_H__