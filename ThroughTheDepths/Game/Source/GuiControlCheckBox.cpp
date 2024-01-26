#include "GuiControlCheckBox.h"
#include "Render.h"
#include "App.h"
#include "Log.h"
#include "Window.h"
#include "Textures.h"
#include "Audio.h"

GuiControlCheckBox::GuiControlCheckBox(uint32 id, SDL_Rect bounds, const char* text)
    : GuiControl(GuiControlType::CHECKBOX, id), isChecked(false), text(text)
{   
    checkBoxNormal = app->tex->Load("Assets/UI/checkBoxNormal.png");
    checkBoxUsed = app->tex->Load("Assets/UI/checkBoxUsed.png");

    checkBox = app->audio->LoadFx("Assets/Audio/Fx/checkBox.wav");
    this->bounds = bounds;
    canClick = true;
    drawBasic = false;

}

GuiControlCheckBox::~GuiControlCheckBox()
{

}

bool GuiControlCheckBox::Update(float dt)
{
    

    return false;
}

bool GuiControlCheckBox::PostUpdate()
{
    if (state != GuiControlState::DISABLED)
    {
        app->input->GetMousePosition(mouseX, mouseY);

        mouseX *= app->win->GetScale();
        mouseY *= app->win->GetScale();

        if (mouseX > bounds.x && mouseX < bounds.x + bounds.w && mouseY > bounds.y && mouseY < bounds.y + bounds.h)
        {
            state = GuiControlState::FOCUSED;

            if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
            {
                state = GuiControlState::PRESSED;
            }
        }

        if (state == GuiControlState::PRESSED)
        {
            app->audio->PlayFx(checkBox);
            isChecked = !isChecked;
            NotifyObserver();
        }

        Draw();
    }
    return false;
}

void GuiControlCheckBox::Draw()
{

    // Draw the checkbox background
    
    if (isChecked)
    {
        // Draw the checkmark
        //app->render->DrawTexture(checkBoxUsed, bounds.x + 5, bounds.y + 5, 0);
        app->render->DrawTexture(checkBoxUsed, bounds.x / app->win->GetScale(), bounds.y / app->win->GetScale(), 0);
        //app->render->DrawLine(bounds.x + 5, bounds.y + 5, bounds.x + bounds.w - 5, bounds.y + bounds.h - 5, 0, 255, 255, 255);
        //app->render->DrawLine(bounds.x + bounds.w - 5, bounds.y + 5, bounds.x + 5, bounds.y + bounds.h - 5, 0, 255, 255, 255);
    }
    else {
        //app->render->DrawTexture(checkBoxNormal, bounds.x, bounds.y, 0);
        app->render->DrawTexture(checkBoxNormal, bounds.x / app->win->GetScale(), bounds.y / app->win->GetScale(), 0);
        //app->render->DrawRectangle(bounds, 150, 150, 250, 255, true, false);
    }

    // Draw the text label
    app->render->DrawText(text.GetString(), bounds.x + bounds.w + 5, bounds.y, 100, bounds.h);
}