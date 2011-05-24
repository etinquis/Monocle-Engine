#include <GUI/gwen/GwenMonocleRenderer.h>
#include <Gwen/Structures.h>
#include <Gwen/TextObject.h>
#include <Gwen/Texture.h>
#include <TextureAsset.h>
#include <FontAsset.h>
#include <Graphics.h>
#include <Assets.h>
#include <iostream>
#include <cmath>

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
            Monocle::Graphics::SetColor( Monocle::Color(color.r/255.f, color.g/255.f, color.b/255.f, color.a/255.f) );
        }

        void GwenRenderer::DrawLine( int x, int y, int a, int b )
        {
            Monocle::Graphics::RenderLine( Monocle::Vector2(x,y), Monocle::Vector2(a,b) );
        }

        void GwenRenderer::DrawFilledRect( Gwen::Rect rect )
        {
            Translate(rect);
            Monocle::Graphics::BindTexture(NULL);
            Monocle::Graphics::RenderQuad( rect.w, rect.h, Monocle::Vector2::zero, Monocle::Vector2::one, Monocle::Vector2( rect.x + (rect.w/2), rect.y + (rect.h / 2) ) );
        }

        void GwenRenderer::LoadTexture( Gwen::Texture *pTexture )
        {
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
            TextureAsset *tex = (TextureAsset *)pTexture->data;
            tex->Unload();
            delete tex;
            tex = NULL;
        }

        void GwenRenderer::LoadFont( Gwen::Font *pFont )
        {
            pFont->realsize = pFont->size * Scale();

            #pragma warning NOT SAFE!
            std::string filename;
            filename.assign(pFont->facename.begin(), pFont->facename.end());

            pFont->data = Monocle::Assets::RequestFont( filename, pFont->realsize );
        }

        void GwenRenderer::FreeFont( Gwen::Font *pFont )
        {
            std::cout << "[GwenRenderer] FreeFont" << std::endl;
            Monocle::FontAsset *font = (Monocle::FontAsset *)pFont->data;

            #pragma warning needs fontAsset->Unload or something
            delete font;
            font = NULL;
        }

        Gwen::Point GwenRenderer::MeasureText( Gwen::Font *pFont, const Gwen::UnicodeString& text )
        {
            if(!pFont->data || std::fabs( pFont->realsize - pFont->size * Scale() ) > 2)
            {
                FreeFont(pFont);
                LoadFont(pFont);
            }

            Monocle::FontAsset *font = (Monocle::FontAsset *)pFont->data;

            Gwen::Point pt;

            #pragma warning NOT SAFE!
            std::string txt;
            txt.assign(text.begin(), text.end());

            if(font)
            {
                pt.x = font->GetTextWidth( txt );
                pt.y = font->GetTextHeight( txt );
            }

            return pt;
        }

        void GwenRenderer::StartClip()
        {
            //std::cout << "[GwenRenderer] StartClip" << std::endl;
        }

        void GwenRenderer::EndClip()
        {
            //std::cout << "[GwenRenderer] EndClip" << std::endl;
        }

        void GwenRenderer::DrawTexturedRect(Gwen::Texture *tex, Gwen::Rect rect, float u1, float v1, float u2, float v2)
        {
            Translate(rect);
            Monocle::Graphics::BindTexture( (TextureAsset*)tex->data );
            Monocle::Graphics::RenderQuad( rect.w, rect.h, Monocle::Vector2( u1, v1 ), Monocle::Vector2::one, Monocle::Vector2( rect.w / 2, rect.h / 2) );
        }

        void GwenRenderer::RenderText( Gwen::Font *font, Gwen::Point pos, const Gwen::UnicodeString& text )
        {
            if(!font->data || std::fabs( font->realsize - font->size * Scale()) > 2)
            {
                FreeFont(font);
                LoadFont(font);
            }

            if(font->data)
            {
                Translate(pos.x, pos.y);

                #pragma warning NOT SAFE!
                std::string txt;
                txt.assign(text.begin(), text.end());

                FontAsset *f = (FontAsset *)font->data;

                Monocle::Graphics::SetBlend(Monocle::BLEND_ALPHA);
                Monocle::Graphics::BindFont((FontAsset*)font->data);
                Monocle::Graphics::RenderText( *f, txt, (float)pos.x, (float)pos.y + f->GetTextHeight(txt) );
            }
        }
    }
}
