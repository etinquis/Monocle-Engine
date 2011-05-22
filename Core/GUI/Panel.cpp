#include "Panel.h"

namespace Monocle
{
    Panel *Panel::currentFocus = NULL;

    Panel::Panel(Vector2 pos, Vector2 sz) : size(sz)
    {
        this->position = pos;
    }

    void Panel::setSize(int x, int y)
    {
        size = Vector2(x,y);
    }

    void Panel::setSize(Vector2 newsize)
    {
        size = newsize;
    }

    bool Panel::hasFocus()
    {
        return currentFocus == this;
    }

    void Panel::Update()
    {
        Entity::Update();
        Vector2 localMousePos = Input::GetWorldMousePosition() - position;
        if(   localMousePos.x < size.x && localMousePos.y < size.y
           && Input::IsMouseButtonPressed(MOUSE_BUTTON_LEFT)
           && currentFocus != this)
        {
            Debug::Log("Changing Panel focus");
            currentFocus = this;
        }

        if(Input::IsKeyPressed(KEY_ESCAPE))
        {
            Debug::Log("Panels have lost focus");
            currentFocus = NULL;
        }
    }

    void Panel::setFocus(Panel *panel)
    {
        currentFocus = panel;
    }
}
