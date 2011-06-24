#ifdef GUI_BERKELIUM

#include <MonocleToolkit.h>
#include <Graphics.h>
#include <Input.h>
#include <TextureAsset.h>
#include <GUI/Berkelium/BerkeliumPanel.h>

#include <berkelium/Berkelium.hpp>
#include <berkelium/Context.hpp>

#define BERK_BPP 4 //the byte width of pixels recieved from Berkelium

namespace Monocle
{
    namespace GUI
    {
        bool BerkeliumPanel::initialised = false;

        BerkeliumPanel::BerkeliumPanel() : Panel(Vector2::zero, Vector2(Monocle::Graphics::GetVirtualWidth(), Monocle::Graphics::GetVirtualHeight()))
        {
            initBerkelium();

            cxt = Berkelium::Context::create();
            win = Berkelium::Window::create(cxt);
            win->resize(size.x, size.y);
            win->setTransparent(false);
            win->setDelegate(this);
            win->focus();

            tex = new TextureAsset();
            tex->Load(NULL, size.x, size.y, Monocle::FILTER_NONE, false, false);
        }

        BerkeliumPanel::BerkeliumPanel(const Vector2 pos, const Vector2 size) : Panel(pos, size)
        {
            initBerkelium();

            cxt = Berkelium::Context::create();
            win = Berkelium::Window::create(cxt);
            win->resize(size.x, size.y);
            win->setTransparent(false);
            win->setDelegate(this);
            win->focus();

            tex = new TextureAsset();
            tex->Load(NULL, size.x, size.y, Monocle::FILTER_NONE, false, false);
        }

        BerkeliumPanel::~BerkeliumPanel()
        {
            delete win;
            win = NULL;
            delete cxt;
            cxt = NULL;

            delete tex;
            tex = NULL;
        }

        void BerkeliumPanel::Update()
        {
            Panel::Update();

            if(hasFocus())
                captureInput();

            Monocle::Vector2 localMousePos = Monocle::Input::GetMousePosition() - position;
            win->mouseMoved(localMousePos.x, localMousePos.y);

            static bool lastState = Monocle::Input::IsMouseButtonHeld(MOUSE_BUTTON_LEFT);
            if(lastState != Monocle::Input::IsMouseButtonHeld(MOUSE_BUTTON_LEFT))
            {
                win->mouseButton(Monocle::MOUSE_BUTTON_LEFT, Monocle::Input::IsMouseButtonHeld(MOUSE_BUTTON_LEFT));
                lastState = Monocle::Input::IsMouseButtonHeld(MOUSE_BUTTON_LEFT);
            }

            win->mouseWheel(0, Monocle::Input::GetMouseScroll());

            Berkelium::update();
        }

        void BerkeliumPanel::onPaint(Berkelium::Window *wini,
                                    const unsigned char *bitmap_in, const Berkelium::Rect &bitmap_rect,
                                    size_t numCopyRects, const Berkelium::Rect *copyRects,
                                    int dx, int dy, const Berkelium::Rect &scroll_rect)
        {
            if(win != wini) return;
            int bmpsize = bitmap_rect.height() * bitmap_rect.width() * BERK_BPP;
            unsigned char *color_corrected_bmp = new unsigned char[bmpsize];
            //copy the bitmap we recieve into a new buffer because we recieve a const and we need to change the pixel format
            //TODO::Change if Monocle adds texture support for more than RGBA format.  In that case, this copying nonsense can be scrapped and the BGRA
            //pixels can be passed in unaltered.
            std::copy(bitmap_in, bitmap_in + bmpsize, color_corrected_bmp);
            //swap the g and r values to prepare to pass into monocle.
            Monocle::BGRAToRGBA(color_corrected_bmp, bitmap_rect.width(), bitmap_rect.height());

            //check if there has been any scrolling.  dx > 0 is scrolling up vertically, dx < 0 scrolling down, dy > 0 scrolling left, dy < 0 scrolling right
            if(dx != 0 || dy != 0)
            {
                //scrolled_rect holds the rect representing the new page position.
                Berkelium::Rect scrolled_rect = scroll_rect.translate(-dx, -dy);

                //scrolled_shared_rect holds the intersection between the two pages.  In other words, this is the part of the page that is still visible after the scroll.
                Berkelium::Rect scrolled_shared_rect = scroll_rect.intersect(scrolled_rect);

                if (scrolled_shared_rect.width() > 0 && scrolled_shared_rect.height() > 0) {
                    //shared_rect holds the final position of the portion of the page we have to move.
                    Berkelium::Rect shared_rect = scrolled_shared_rect.translate(dx, dy);

                    tex->CopyRect(//from
                                  Vector2(scrolled_shared_rect.left(), scrolled_shared_rect.top()),
                                  //to
                                  Vector2(shared_rect.left(), shared_rect.top()),
                                  //size
                                  Vector2(scrolled_shared_rect.width(), scrolled_shared_rect.height()));
                }
            }

            //CopyRects contains all the rect portions of bitmap_in that are valid and should be drawn. (all the new or changed parts of the page)
            for(size_t i = 0; i < numCopyRects; i++)
            {
                int w = copyRects[i].width();
                int h = copyRects[i].height();
                int top = copyRects[i].top() - bitmap_rect.top();
                int left = copyRects[i].left() - bitmap_rect.left();

                //allocate a buffer to hold the data that will be used to update the texture
                unsigned char *buff = new unsigned char[w * h * 4];
                //j is the pixel line number of the rect we're trying to copy
                for(int j = 0; j < h; j++)
                {
                    const unsigned char *begin = color_corrected_bmp + (left + ((j + top) * bitmap_rect.width())) * 4;
                    std::copy(begin, begin + w * 4, buff + j * w * 4);
                }
                tex->UpdateRect(buff, Vector2(copyRects[i].left(), copyRects[i].top()), Vector2(w, h));

                delete[] buff;
            }

            delete[] color_corrected_bmp;
        }

        void BerkeliumPanel::Render() //const
        {
            Panel::Render();

            if(tex)
            {
                Monocle::Graphics::PushMatrix();

                //this->scene->GetCamera()->ApplyMatrix();
                //Monocle::Graphics::Translate(position + Monocle::Vector2(size.x / 2, size.y /2));
                ApplyMatrix();

                Monocle::Graphics::BindTexture(tex);

                Monocle::Graphics::SetBlend(Monocle::BLEND_ALPHA);
                Monocle::Graphics::RenderQuad(size.x, size.y);

                Monocle::Graphics::PopMatrix();
            }
        }

        void BerkeliumPanel::onConsoleMessage(Berkelium::Window *win, Berkelium::WideString message, Berkelium::WideString sourceID, int line_no) const
        {
            std::wcout << "[Berkelium Panel] Console message: (" << sourceID << ") " << message << std::endl;
        }

        void BerkeliumPanel::onJavascriptCallback(Berkelium::Window* wini,
                                                  void *replyMsg,
                                                  Berkelium::URLString origin,
                                                  Berkelium::WideString funcName,
                                                  Berkelium::Script::Variant *args, size_t numArgs)
        {
            std::cout << "onJavascriptCallback" << std::endl;

        }

        void BerkeliumPanel::setUrl(const std::string url) const
        {
            win->navigateTo(url.data(), url.length());
        }

        void BerkeliumPanel::captureInput() const
        {
            bool shift = Monocle::Input::IsKeyHeld(Monocle::KEY_LSHIFT);

            if(Monocle::Input::IsKeyPressed(Monocle::KEY_A)) { win->textEvent( shift ? L"A" : L"a", 1);}
            if(Monocle::Input::IsKeyPressed(Monocle::KEY_B)) { win->textEvent( shift ? L"B" : L"b", 1);}
            if(Monocle::Input::IsKeyPressed(Monocle::KEY_C)) { win->textEvent( shift ? L"C" : L"c", 1);}
            if(Monocle::Input::IsKeyPressed(Monocle::KEY_D)) { win->textEvent( shift ? L"D" : L"d", 1);}
            if(Monocle::Input::IsKeyPressed(Monocle::KEY_E)) { win->textEvent( shift ? L"E" : L"e", 1);}
            if(Monocle::Input::IsKeyPressed(Monocle::KEY_F)) { win->textEvent( shift ? L"F" : L"f", 1);}
            if(Monocle::Input::IsKeyPressed(Monocle::KEY_G)) { win->textEvent( shift ? L"G" : L"g", 1);}
            if(Monocle::Input::IsKeyPressed(Monocle::KEY_H)) { win->textEvent( shift ? L"H" : L"h", 1);}
            if(Monocle::Input::IsKeyPressed(Monocle::KEY_I)) { win->textEvent( shift ? L"I" : L"i", 1);}
            if(Monocle::Input::IsKeyPressed(Monocle::KEY_J)) { win->textEvent( shift ? L"J" : L"j", 1);}
            if(Monocle::Input::IsKeyPressed(Monocle::KEY_K)) { win->textEvent( shift ? L"K" : L"k", 1);}
            if(Monocle::Input::IsKeyPressed(Monocle::KEY_L)) { win->textEvent( shift ? L"L" : L"l", 1);}
            if(Monocle::Input::IsKeyPressed(Monocle::KEY_M)) { win->textEvent( shift ? L"M" : L"m", 1);}
            if(Monocle::Input::IsKeyPressed(Monocle::KEY_N)) { win->textEvent( shift ? L"N" : L"n", 1);}
            if(Monocle::Input::IsKeyPressed(Monocle::KEY_O)) { win->textEvent( shift ? L"O" : L"o", 1);}
            if(Monocle::Input::IsKeyPressed(Monocle::KEY_P)) { win->textEvent( shift ? L"P" : L"p", 1);}
            if(Monocle::Input::IsKeyPressed(Monocle::KEY_Q)) { win->textEvent( shift ? L"Q" : L"q", 1);}
            if(Monocle::Input::IsKeyPressed(Monocle::KEY_R)) { win->textEvent( shift ? L"R" : L"r", 1);}
            if(Monocle::Input::IsKeyPressed(Monocle::KEY_S)) { win->textEvent( shift ? L"S" : L"s", 1);}
            if(Monocle::Input::IsKeyPressed(Monocle::KEY_T)) { win->textEvent( shift ? L"T" : L"t", 1);}
            if(Monocle::Input::IsKeyPressed(Monocle::KEY_U)) { win->textEvent( shift ? L"U" : L"u", 1);}
            if(Monocle::Input::IsKeyPressed(Monocle::KEY_V)) { win->textEvent( shift ? L"V" : L"v", 1);}
            if(Monocle::Input::IsKeyPressed(Monocle::KEY_W)) { win->textEvent( shift ? L"W" : L"w", 1);}
            if(Monocle::Input::IsKeyPressed(Monocle::KEY_X)) { win->textEvent( shift ? L"X" : L"x", 1);}
            if(Monocle::Input::IsKeyPressed(Monocle::KEY_Y)) { win->textEvent( shift ? L"Y" : L"y", 1);}
            if(Monocle::Input::IsKeyPressed(Monocle::KEY_Z)) { win->textEvent( shift ? L"Z" : L"z", 1);}

            if(Monocle::Input::IsKeyPressed(Monocle::KEY_EQUALS)) { win->textEvent( shift ? L"+" : L"=", 1);}
            if(Monocle::Input::IsKeyPressed(Monocle::KEY_SLASH)) { win->textEvent( shift ? L"?" : L"/", 1);}
            if(Monocle::Input::IsKeyPressed(Monocle::KEY_BACKSLASH)) { win->textEvent( shift ? L"|" : L"\\", 1);}
            if(Monocle::Input::IsKeyPressed(Monocle::KEY_BACKQUOTE)) { win->textEvent( shift ? L"~" : L"`", 1);}
            if(Monocle::Input::IsKeyPressed(Monocle::KEY_LEFTBRACKET)) { win->textEvent( shift ? L"{" : L"[", 1);}
            if(Monocle::Input::IsKeyPressed(Monocle::KEY_RIGHTBRACKET)) { win->textEvent( shift ? L"}" : L"]", 1);}
            if(Monocle::Input::IsKeyPressed(Monocle::KEY_SEMICOLON)) { win->textEvent( shift ? L":" : L";", 1);}
            if(Monocle::Input::IsKeyPressed(Monocle::KEY_APOSTROPHE)) { win->textEvent( shift ? L"\"" : L"\'", 1); }
            if(Monocle::Input::IsKeyPressed(Monocle::KEY_PERIOD)) { win->textEvent( shift ? L">" : L".", 1);}
            if(Monocle::Input::IsKeyPressed(Monocle::KEY_COMMA)) { win->textEvent( shift ? L"<" : L",", 1);}
            if(Monocle::Input::IsKeyPressed(Monocle::KEY_1)) { win->textEvent( shift ? L"!" : L"1", 1);}
            if(Monocle::Input::IsKeyPressed(Monocle::KEY_2)) { win->textEvent( shift ? L"@" : L"2", 1);}
            if(Monocle::Input::IsKeyPressed(Monocle::KEY_3)) { win->textEvent( shift ? L"#" : L"3", 1);}
            if(Monocle::Input::IsKeyPressed(Monocle::KEY_4)) { win->textEvent( shift ? L"$" : L"4", 1);}
            if(Monocle::Input::IsKeyPressed(Monocle::KEY_5)) { win->textEvent( shift ? L"%" : L"5", 1);}
            if(Monocle::Input::IsKeyPressed(Monocle::KEY_6)) { win->textEvent( shift ? L"^" : L"6", 1);}
            if(Monocle::Input::IsKeyPressed(Monocle::KEY_7)) { win->textEvent( shift ? L"&" : L"7", 1);}
            if(Monocle::Input::IsKeyPressed(Monocle::KEY_8)) { win->textEvent( shift ? L"*" : L"8", 1);}
            if(Monocle::Input::IsKeyPressed(Monocle::KEY_9)) { win->textEvent( shift ? L"(" : L"9", 1);}
            if(Monocle::Input::IsKeyPressed(Monocle::KEY_0)) { win->textEvent( shift ? L")" : L"0", 1);}

            if(Monocle::Input::IsKeyPressed(Monocle::KEY_SPACE)) {win->textEvent( L" ", 1 );}

            if(Monocle::Input::IsKeyPressed(Monocle::KEY_BACKSPACE)) { win->keyEvent(true, 0, '\b', 0); win->textEvent(L"a", 1); }

            if(Monocle::Input::IsKeyPressed(Monocle::KEY_RETURN))
            {
                win->keyEvent(true, 0, '\r', 0);
            }
        }

        void BerkeliumPanel::setSize(int x, int y)
        {
            Panel::setSize(x,y);
            win->resize(x,y);
        }

        void BerkeliumPanel::setSize(Monocle::Vector2 newsize)
        {
            Panel::setSize(newsize);
            win->resize(newsize.x, newsize.y);
        }

        void BerkeliumPanel::initBerkelium()
        {
            if(!initialised)
            {
                Monocle::Debug::Log("Berkelium initialising");
                //the parameter to init is the directory to use for all the data files that chrome needs to create (bookmarks, history, etc)
                //an empty filestring causes berkelium to use a temporary directory.
                Berkelium::init(Berkelium::FileString::empty());
                initialised = true;
            }
        }
    }
}

#endif
