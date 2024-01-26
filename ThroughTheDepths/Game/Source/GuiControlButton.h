#ifndef __GUICONTROLBUTTON_H__
#define __GUICONTROLBUTTON_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

class GuiControlButton : public GuiControl
{

public:

	GuiControlButton(uint32 id, SDL_Rect bounds, const char* text);
	virtual ~GuiControlButton();
	bool Start();
	// Called each loop iteration
	bool Update(float dt);
	bool PostUpdate();

	SDL_Texture* normalButton;
	SDL_Texture* pressedButton;
	SDL_Texture* hoverButton;

	uint selectButton;
	uint clickButton;

	bool sonido;
	bool sonido2;
private:

	int mouseX, mouseY;
	unsigned int click;

	bool canClick = true;
	bool drawBasic = false;
};

#endif // __GUICONTROLBUTTON_H__