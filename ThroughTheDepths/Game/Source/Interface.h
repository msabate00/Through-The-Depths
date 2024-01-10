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

	// Called before quitting
	bool CleanUp();

	bool OnGuiMouseClickEvent(GuiControl* control);


	
	//SDL_Texture* img;
	//SDL_Texture* fondo1;
	//SDL_Texture* fondo2;
	//SDL_Texture* fondo0;
private:
	
	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;



	GuiControlButton* gcButtom;
	

	
};

#endif // __Interface_H__