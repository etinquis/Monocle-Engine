#include "Panel.h"

#include <Input.h>
#include <Graphics.h>
#include <Component/Entity/Transform.h>

namespace Monocle
{
    Panel *Panel::currentFocus = NULL;

    Panel::Panel(Vector2 pos, Vector2 sz) : size(sz)
    {
		transform = AddComponent<Transform>(Transform::InitParams(pos));
    }

    /*void Panel::SetSize(int x, int y)
    {
        size = Vector2(x,y);
    }*/

    /*void Panel::SetSize(Vector2 newsize)
    {
        size = newsize;
    }*/

    bool Panel::HasFocus()
    {
        return currentFocus == this;
    }

    void Panel::Update()
    {
        Entity::Update();

        Vector2 localMousePos = Input::GetWorldMousePosition() - transform->position;
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

    void Panel::Render()
    {
        Entity::Render();
    }

    void Panel::ApplyMatrix()
    {
        Vector2 pos = Monocle::Graphics::GetScreenCenter();

        Graphics::Translate(pos);

//        if (followCamera == Vector2::zero || (Debug::render && Debug::selectedEntity != this && IsDebugLayer()))
//			Graphics::Translate(position.x, position.y, depth);
//		else
//			Graphics::Translate(scene->GetCamera()->position * followCamera + position * (Vector2::one - followCamera));
    }

    void Panel::SetFocus(Panel *panel)
    {
        currentFocus = panel;
    }
}
