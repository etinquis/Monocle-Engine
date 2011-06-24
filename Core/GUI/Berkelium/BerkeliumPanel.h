#pragma once

#ifndef MONOCLE_GUI_BERKELIUMPANEL
#define MONOCLE_GUI_BERKELIUMPANEL

#include <GUI/Panel.h>
#include <berkelium/WindowDelegate.hpp>
#include <GUI/Berkelium/JSBinding/JSFunctionBinding.h>

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
        //! \brief An implementation of the Berkelium Chromium library into Monocle.
        //!
        //!
        //!
        //!
        class BerkeliumPanel : public Monocle::Panel, public Berkelium::WindowDelegate
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
            template<typename Function>
            inline void BindFunction(const std::wstring name, //! < \param name The name of the javascript function to bind to.
                                    const Function func)      //! < \param func The function to be bound.
            {
                //bindDispatch.AddCallback(name, func);
                win->addBindOnStartLoading(Berkelium::WideString::point_to(name), Berkelium::Script::Variant::bindFunction(Berkelium::WideString::point_to(name), false));
            }

            virtual void setSize(int x, int y);
            virtual void setSize(Monocle::Vector2 newsize);


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

            //Checks input to determine if any keys have been pressed and forwards them to berkelium
            void captureInput() const;

            //whether or not berkelium as a whole has been initialised
            static bool initialised;
            //Initialises berkelium if it hasn't been started yet.
            void initBerkelium();
        };
    }
}

#endif
