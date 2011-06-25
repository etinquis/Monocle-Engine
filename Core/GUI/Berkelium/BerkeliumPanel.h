#pragma once

#ifndef MONOCLE_GUI_BERKELIUMPANEL
#define MONOCLE_GUI_BERKELIUMPANEL

#include <GUI/Panel.h>
#include <berkelium/WindowDelegate.hpp>
#include <GUI/Berkelium/JSBinding/JSFunctionBinding.h>
#include <map>

namespace Berkelium
{
    class Context;
    class Window;
}

namespace Monocle
{
    class TextureAsset;

    namespace GUI
    {
        //! \brief An implementation of the Berkelium Chromium library in Monocle.
        class BerkeliumPanel : public Monocle::Panel, public Berkelium::WindowDelegate, public Monocle::Input::EventHandler
        {
        public:
            //! Default constructor.  Panel will be positioned at (0,0) and will fill the window.
            BerkeliumPanel();
            //! Constructor initialising position and size.  Calls Panel constructor for pos and size.
            //! \sa Panel::Panel() */
            BerkeliumPanel(const Vector2 pos,   //!< \param pos The position at which the panel should be placed. */
                           const Vector2 size); //!< \param size How large the panel should be drawn. */
            //! Virtual destructor.
            virtual ~BerkeliumPanel();

            //! Entity Update function. Handles input and updating Berkelium.  Should not need to be called externally.
            //! \sa Entity::Update()
            virtual void Update();
            //! Entity Render function. Renders the texture containing the berkelium window in the viewport.
            //! \sa Entity::Render()
            virtual void Render(); //const;

            //!Sets the url of the panel to a new page.
            virtual void setUrl(const std::string url) const; //! <\param url The url to navigate to.  Must be preceded by a protocol (e.g. file://, http://, etc).

            //! Bind a callback to a javascript function
			void BindFunction(const JSBinding::JSFunctionCallback& func);      //! < \param func The function to be bound.

            virtual void SetSize(int x, int y);
            virtual void SetSize(Monocle::Vector2 newsize);

			virtual void OnKeyPress(Monocle::KeyCode key);

        protected:
            Berkelium::Window *win;
            virtual void onJavascriptCallback(Berkelium::Window* wini,
                  void *replyMsg,
                  Berkelium::URLString origin,
                  Berkelium::WideString funcName,
                  Berkelium::Script::Variant *args, size_t numArgs);
            virtual void onPaint(Berkelium::Window *wini,
                    const unsigned char *bitmap_in, const Berkelium::Rect &bitmap_rect,
                    size_t numCopyRects, const Berkelium::Rect *copyRects,
                    int dx, int dy, const Berkelium::Rect &scroll_rect);
            virtual void onConsoleMessage(Berkelium::Window *win, Berkelium::WideString message, Berkelium::WideString sourceID, int line_no) const;
        private:
            //texture asset holding the texture used to draw the window.
            TextureAsset *tex;
            //holds the context for the berkelium window in *win.  Contexts are rendering surfaces, so
            //sharing one means drawing to the same surface.
            Berkelium::Context *cxt;
            
            typedef std::multimap<std::wstring, JSBinding::JSFunctionCallback> JSCallbackList;
            JSCallbackList jsfuncs;

            //whether or not berkelium as a whole has been initialised
            static bool initialised;
            //Initialises berkelium if it hasn't been started yet.
            void initBerkelium();
			void initInputMap();
        };
    }
}

#endif
