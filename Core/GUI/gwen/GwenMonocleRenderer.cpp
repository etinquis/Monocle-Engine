#include <GUI/gwen/GwenMonocleRenderer.h>
#include <Gwen/Structures.h>
#include <Gwen/TextObject.h>
#include <Gwen/Texture.h>
#include <TextureAsset.h>
#include <FontAsset.h>
#include <Graphics.h>
#include <Assets.h>
#include <iostream>

namespace Monocle
{
    namespace GUI
    {
        GwenRenderer::GwenRenderer() {}
        GwenRenderer::~GwenRenderer() {}

        void GwenRenderer::Begin() {}
        void GwenRenderer::End() {}

        void GwenRenderer::SetDrawColor( Gwen::Color color )
        {
            std::cout << "[GwenRenderer] SetDrawColor" << std::endl;
            Monocle::Graphics::SetColor( Monocle::Color(color.r/255.f, color.g/255.f, color.b/255.f, color.a/255.f) );
        }

        void GwenRenderer::DrawLine( int x, int y, int a, int b )
        {
            std::cout << "[GwenRenderer] DrawLine" << std::endl;
            Monocle::Graphics::RenderLine( Monocle::Vector2(x,y), Monocle::Vector2(a,b) );
        }

        void GwenRenderer::DrawFilledRect( Gwen::Rect rect )
        {
            std::cout << "[GwenRenderer] DrawFilledRect" << std::endl;
            Monocle::Graphics::RenderQuad( rect.w, rect.h, Monocle::Vector2::zero, Monocle::Vector2::one, Monocle::Vector2( rect.x + (rect.w/2), rect.y + (rect.h / 2) ) );
        }

        void GwenRenderer::LoadTexture( Gwen::Texture *pTexture )
        {
            std::cout << "[GwenRenderer] LoadTexture" << std::endl;
            Monocle::TextureAsset *tex = Monocle::Assets::RequestTexture( pTexture->name.Get() );

            if(tex)
            {
                pTexture->data = tex;
                pTexture->width = tex->width;
                pTexture->height = tex->height;
            }
            else
            {
                pTexture->failed = true;
            }
        }

        void GwenRenderer::FreeTexture( Gwen::Texture *pTexture )
        {
            std::cout << "[GwenRenderer] FreeTexture" << std::endl;
            TextureAsset *tex = (TextureAsset *)pTexture->data;
            tex->Unload();
            delete tex;
            tex = NULL;
        }

        void GwenRenderer::LoadFont( Gwen::Font *pFont )
        {
            std::cout << "[GwenRenderer] LoadFont" << std::endl;
            pFont->realsize = pFont->size * Scale();
            //Monocle::FontAsset *font = Monocle::Assets::RequestFont( pFont->facename.data(), pFont->size );
            //pFont->data = font;
        }

        void GwenRenderer::FreeFont( Gwen::Font *pFont )
        {
            std::cout << "[GwenRenderer] FreeFont" << std::endl;
            Monocle::FontAsset *font = (Monocle::FontAsset *)pFont->data;

            // TODO:: needs fontAsset->Unload or something
            delete font;
            font = NULL;
        }

        Gwen::Point GwenRenderer::MeasureText( Gwen::Font *pFont, const Gwen::UnicodeString& text )
        {
            std::cout << "[GwenRenderer] MeasureText" << std::endl;
            Monocle::FontAsset *font = (Monocle::FontAsset *)pFont->data;

            Gwen::Point pt;

            #pragma warning Not done
            //pt.x = font->GetTextWidth( text.data() );
            //pt.y = font->GetTextHeight( text.data() );

            return pt;
        }

        void GwenRenderer::StartClip()
        {
            std::cout << "[GwenRenderer] StartClip" << std::endl;
        }

        void GwenRenderer::EndClip()
        {
            std::cout << "[GwenRenderer] EndClip" << std::endl;
        }

        void GwenRenderer::DrawTexturedRect(Gwen::Texture *tex, Gwen::Rect rect, float u1, float v1, float u2, float v2)
        {
            std::cout << "[GwenRenderer] DrawTexturedRect" << std::endl;
            Translate(rect);
            Monocle::Graphics::BindTexture( (TextureAsset*)tex->data );
            Monocle::Graphics::RenderQuad( rect.w, rect.h, Monocle::Vector2( u1, v1 ), Monocle::Vector2::one, Monocle::Vector2( rect.w / 2, rect.h / 2) );
        }

        void GwenRenderer::RenderText( Gwen::Font *font, Gwen::Point pos, const Gwen::UnicodeString& text )
        {
            std::cout << "[GwenRenderer] RenderText" << std::endl;
            //Monocle::Graphics::RenderText( (FontAsset *)font->data, text, pos.x, pos.y );
        }
    }
}
