#pragma once

#ifndef MONOCLE_GUI_PANEL
#define MONOCLE_GUI_PANEL

#include "Entity.h"
#include <Input.h>

namespace Monocle
{
    class Panel : public Entity
    {
    public:
        Panel(Vector2 position = Vector2::zero, Vector2 size = Vector2(100,100));
        virtual ~Panel() { }

        virtual void Update();
        virtual void Render();

        //virtual void SetSize(Vector2 size);
        //virtual void SetSize(int x, int y);

        bool HasFocus();
        static void SetFocus(Panel *p);
    protected:
        void ApplyMatrix();

        Vector2 size;
        static Panel *currentFocus;
        
		Transform *transform;

        std::map<Monocle::KeyCode, wchar_t> inputMap;
    };
}

#endif
