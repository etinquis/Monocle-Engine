#pragma once

#ifndef MONOCLE_GUI_PANEL
#define MONOCLE_GUI_PANEL

#include "Entity.h"

namespace Monocle
{
    class Panel : public Entity
    {
    public:
        Panel(Vector2 position = Vector2::zero, Vector2 size = Vector2(100,100));
        virtual ~Panel() { }

        virtual void Update();
        virtual void Render();

        virtual void setSize(Vector2 size);
        virtual void setSize(int x, int y);

        bool hasFocus();
        static void setFocus(Panel *p);
    protected:
        void ApplyMatrix();

        Vector2 size;
        static Panel *currentFocus;
    };
}

#endif
