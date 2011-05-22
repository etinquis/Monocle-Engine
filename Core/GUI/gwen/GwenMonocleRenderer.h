#pragma once

#ifndef MONOCLE_GUI_GWENRENDERER
#define MONOCLE_GUI_GWENRENDERER

#include <Gwen/BaseRender.h>

namespace Monocle
{
    namespace GUI
    {
        class GwenRenderer : public Gwen::Renderer::Base
        {
        public:
            GwenRenderer();
            virtual ~GwenRenderer();

            virtual void Begin();
            virtual void End();

            virtual void SetDrawColor( Gwen::Color color );

            virtual void DrawLine( int x, int y, int a, int b);
            virtual void DrawFilledRect( Gwen::Rect rect );


            virtual void StartClip();
            virtual void EndClip();

            virtual void LoadTexture( Gwen::Texture *pTexture );
            virtual void FreeTexture( Gwen::Texture *pTexture );
            virtual void DrawTexturedRect( Gwen::Texture *pTexture, Gwen::Rect pTargetRect, float u1=0.0f, float v1=0.0f, float u2=1.0f, float v2=1.0f );

            virtual void LoadFont( Gwen::Font *pFont );
            virtual void FreeFont( Gwen::Font *pFont );
            virtual void RenderText( Gwen::Font *pFont, Gwen::Point pos, const Gwen::UnicodeString& text );

            virtual Gwen::Point MeasureText( Gwen::Font *pFont, const Gwen::UnicodeString& text );
        };
    }
}

#endif
