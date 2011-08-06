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
			initInputMap();

            cxt = Berkelium::Context::create();
            win = Berkelium::Window::create(cxt);
            win->resize(size.x, size.y);
            win->setTransparent(false);
            win->setDelegate(this);
            win->focus();

			Monocle::Input::AddHandler(this);

            tex = new TextureAsset();
            tex->Load(NULL, size.x, size.y, Monocle::FILTER_NONE, false, false);
        }

        BerkeliumPanel::BerkeliumPanel(const Vector2 pos, const Vector2 size) : Panel(pos, size)
        {
            initBerkelium();
			initInputMap();
			
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
        
        void BerkeliumPanel::BindFunction(const JSBinding::JSFunctionCallback& func)
        {
        	jsfuncs.insert( std::pair<std::wstring, JSBinding::JSFunctionCallback>(func.name, func) );
			win->addBindOnStartLoading(Berkelium::WideString::point_to(func.name), Berkelium::Script::Variant::bindFunction(Berkelium::WideString::point_to(func.name), false));
        }

        void BerkeliumPanel::Update()
        {
            Panel::Update();

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
            //Panel::Render();
            
            if(tex)
            {
                Monocle::Graphics::PushMatrix();

                //this->scene->GetCamera()->ApplyMatrix();
                //Monocle::Graphics::Translate(position + Monocle::Vector2(size.x / 2, size.y /2));
                ApplyMatrix();
                //Camera *camera = scene->GetActiveCamera();
				//if (!camera)
				//	camera = scene->GetMainCamera();
				//if (camera != NULL)
				//	Graphics::Translate(camera->position);

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
        	std::pair<JSCallbackList::iterator, JSCallbackList::iterator> funcRange = jsfuncs.equal_range(funcName.data());
        	JSCallbackList::iterator it = funcRange.first;
        	
        	JSBinding::JSFunctionCallback *func;
        	while( it != funcRange.second && it->second.numArgs <= numArgs )
        	{
        		func = &it->second;
        		it++;
        	}
        	
        	if(func)
				(*func)(std::vector<Berkelium::Script::Variant>(&args[0], &args[numArgs]));
        }

        void BerkeliumPanel::setUrl(const std::string url) const
        {
            win->navigateTo(url.data(), url.length());
        }

		void BerkeliumPanel::OnKeyPress(Monocle::KeyCode key)
		{
			bool shift = Monocle::Input::IsKeyHeld(Monocle::KEY_LSHIFT) || Monocle::Input::IsKeyHeld(Monocle::KEY_RSHIFT);
			
			if(inputMap[key] - L'a' > -26)
			{
				wchar_t ch = inputMap[key] - ((97-65) * shift);
				win->textEvent( &ch, 1 );
			}
			else
			{
				switch(key)
				{
					case Monocle::KEY_EQUALS:
						win->textEvent( shift ? L"+" : L"=", 1); break;
					case Monocle::KEY_SLASH:
						win->textEvent( shift ? L"?" : L"/", 1); break;
					case Monocle::KEY_BACKSLASH:
						win->textEvent( shift ? L"|" : L"\\", 1); break;
					case Monocle::KEY_BACKQUOTE:
						win->textEvent( shift ? L"~" : L"`", 1); break;
					case Monocle::KEY_LEFTBRACKET:
						win->textEvent( shift ? L"{" : L"[", 1); break;
					case Monocle::KEY_RIGHTBRACKET:
						win->textEvent( shift ? L"}" : L"]", 1); break;
					case Monocle::KEY_SEMICOLON:
						win->textEvent( shift ? L":" : L";", 1); break;
					case Monocle::KEY_APOSTROPHE:
						win->textEvent( shift ? L"\"" : L"\'", 1); break;
					case Monocle::KEY_PERIOD:
						win->textEvent( shift ? L">" : L".", 1); break;
					case Monocle::KEY_COMMA:
						win->textEvent( shift ? L"<" : L",", 1); break;
					case Monocle::KEY_1:
						win->textEvent( shift ? L"!" : L"1", 1); break;
					case Monocle::KEY_2:
						win->textEvent( shift ? L"@" : L"2", 1); break;
					case Monocle::KEY_3:
						win->textEvent( shift ? L"#" : L"3", 1); break;
					case Monocle::KEY_4:
						win->textEvent( shift ? L"$" : L"4", 1); break;
					case Monocle::KEY_5:
						win->textEvent( shift ? L"%" : L"5", 1); break;
					case Monocle::KEY_6:
						win->textEvent( shift ? L"^" : L"6", 1); break;
					case Monocle::KEY_7:
						win->textEvent( shift ? L"&" : L"7", 1); break;
					case Monocle::KEY_8:
						win->textEvent( shift ? L"*" : L"8", 1); break;
					case Monocle::KEY_9:
						win->textEvent( shift ? L"(" : L"9", 1); break;
					case Monocle::KEY_0:
						win->textEvent( shift ? L")" : L"0", 1); break;
						
					case Monocle::KEY_SPACE:
						win->textEvent( L" ", 1 ); break;
					case Monocle::KEY_BACKSPACE:
						win->keyEvent(true, 0, '\b', 0); win->textEvent(L"a", 1); break;
					case Monocle::KEY_RETURN:
						win->keyEvent(true, 0, '\r', 0); break;
				}
			}
		}

        void BerkeliumPanel::SetSize(int x, int y)
        {
            Panel::SetSize(x,y);
            win->resize(x,y);
        }

        void BerkeliumPanel::SetSize(Monocle::Vector2 newsize)
        {
            Panel::SetSize(newsize);
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
        
        void BerkeliumPanel::initInputMap()
        {
        	inputMap[KEY_A] = L'a';
        	inputMap[KEY_B] = L'b';
        	inputMap[KEY_C] = L'c';
        	inputMap[KEY_D] = L'd';
        	inputMap[KEY_E] = L'e';
        	inputMap[KEY_F] = L'f';
        	inputMap[KEY_G] = L'g';
        	inputMap[KEY_H] = L'h';
        	inputMap[KEY_I] = L'i';
        	inputMap[KEY_J] = L'j';
        	inputMap[KEY_K] = L'k';
        	inputMap[KEY_L] = L'l';
        	inputMap[KEY_M] = L'm';
        	inputMap[KEY_N] = L'n';
        	inputMap[KEY_O] = L'o';
        	inputMap[KEY_P] = L'p';
        	inputMap[KEY_Q] = L'q';
        	inputMap[KEY_R] = L'r';
        	inputMap[KEY_S] = L's';
        	inputMap[KEY_T] = L't';
        	inputMap[KEY_U] = L'u';
        	inputMap[KEY_V] = L'v';
        	inputMap[KEY_W] = L'w';
        	inputMap[KEY_X] = L'x';
        	inputMap[KEY_Y] = L'y';
        	inputMap[KEY_Z] = L'z';
        }
    }
}

#endif
