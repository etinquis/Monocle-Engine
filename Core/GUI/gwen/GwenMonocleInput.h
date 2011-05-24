#include "Gwen/InputHandler.h"
#include "Gwen/Gwen.h"
#include "Gwen/Controls/Canvas.h"

namespace Monocle
{
    namespace GUI
    {
        class GwenInput
        {
        public:

            GwenInput()
            {
                m_Canvas = NULL;
            }

            void Initialize( Gwen::Controls::Canvas* c )
            {
                m_Canvas = c;
            }

            bool ProcessInput( )
            {
                if (!m_Canvas) return false;

                m_Canvas->InputKey(Gwen::Key::Return, Monocle::Input::IsKeyPressed(Monocle::KEY_RETURN));
                m_Canvas->InputKey(Gwen::Key::Backspace, Monocle::Input::IsKeyPressed(Monocle::KEY_BACKSPACE));
                m_Canvas->InputKey(Gwen::Key::Delete, Monocle::Input::IsKeyPressed(Monocle::KEY_DELETE));
                m_Canvas->InputKey(Gwen::Key::Left, Monocle::Input::IsKeyPressed(Monocle::KEY_LEFT));
                m_Canvas->InputKey(Gwen::Key::Right, Monocle::Input::IsKeyPressed(Monocle::KEY_UP));
                m_Canvas->InputKey(Gwen::Key::Down, Monocle::Input::IsKeyPressed(Monocle::KEY_DOWN));
                m_Canvas->InputKey(Gwen::Key::Shift, Monocle::Input::IsKeyPressed(Monocle::KEY_LSHIFT));
                m_Canvas->InputKey(Gwen::Key::Shift, Monocle::Input::IsKeyPressed(Monocle::KEY_RSHIFT));
                m_Canvas->InputKey(Gwen::Key::Tab, Monocle::Input::IsKeyPressed(Monocle::KEY_TAB));
                m_Canvas->InputKey(Gwen::Key::Space, Monocle::Input::IsKeyPressed(Monocle::KEY_SPACE));
                m_Canvas->InputKey(Gwen::Key::Home, Monocle::Input::IsKeyPressed(Monocle::KEY_HOME));
                m_Canvas->InputKey(Gwen::Key::End, Monocle::Input::IsKeyPressed(Monocle::KEY_END));
                m_Canvas->InputKey(Gwen::Key::Control, Monocle::Input::IsKeyPressed(Monocle::KEY_LCTRL));
                m_Canvas->InputKey(Gwen::Key::Control, Monocle::Input::IsKeyPressed(Monocle::KEY_RCTRL));
                m_Canvas->InputKey(Gwen::Key::Escape, Monocle::Input::IsKeyPressed(Monocle::KEY_ESCAPE));
                m_Canvas->InputKey(Gwen::Key::Alt, Monocle::Input::IsKeyPressed(Monocle::KEY_LALT));
                m_Canvas->InputKey(Gwen::Key::Alt, Monocle::Input::IsKeyPressed(Monocle::KEY_RALT));


                bool shift = Monocle::Input::IsKeyHeld(Monocle::KEY_LSHIFT);

                if(Monocle::Input::IsKeyPressed(Monocle::KEY_A)) { m_Canvas->InputCharacter( shift ? L'A' : L'a');}
                if(Monocle::Input::IsKeyPressed(Monocle::KEY_B)) { m_Canvas->InputCharacter( shift ? L'B' : L'b');}
                if(Monocle::Input::IsKeyPressed(Monocle::KEY_C)) { m_Canvas->InputCharacter( shift ? L'C' : L'c');}
                if(Monocle::Input::IsKeyPressed(Monocle::KEY_D)) { m_Canvas->InputCharacter( shift ? L'D' : L'd');}
                if(Monocle::Input::IsKeyPressed(Monocle::KEY_E)) { m_Canvas->InputCharacter( shift ? L'E' : L'e');}
                if(Monocle::Input::IsKeyPressed(Monocle::KEY_F)) { m_Canvas->InputCharacter( shift ? L'F' : L'f');}
                if(Monocle::Input::IsKeyPressed(Monocle::KEY_G)) { m_Canvas->InputCharacter( shift ? L'G' : L'g');}
                if(Monocle::Input::IsKeyPressed(Monocle::KEY_H)) { m_Canvas->InputCharacter( shift ? L'H' : L'h');}
                if(Monocle::Input::IsKeyPressed(Monocle::KEY_I)) { m_Canvas->InputCharacter( shift ? L'I' : L'i');}
                if(Monocle::Input::IsKeyPressed(Monocle::KEY_J)) { m_Canvas->InputCharacter( shift ? L'J' : L'j');}
                if(Monocle::Input::IsKeyPressed(Monocle::KEY_K)) { m_Canvas->InputCharacter( shift ? L'K' : L'k');}
                if(Monocle::Input::IsKeyPressed(Monocle::KEY_L)) { m_Canvas->InputCharacter( shift ? L'L' : L'l');}
                if(Monocle::Input::IsKeyPressed(Monocle::KEY_M)) { m_Canvas->InputCharacter( shift ? L'M' : L'm');}
                if(Monocle::Input::IsKeyPressed(Monocle::KEY_N)) { m_Canvas->InputCharacter( shift ? L'N' : L'n');}
                if(Monocle::Input::IsKeyPressed(Monocle::KEY_O)) { m_Canvas->InputCharacter( shift ? L'O' : L'o');}
                if(Monocle::Input::IsKeyPressed(Monocle::KEY_P)) { m_Canvas->InputCharacter( shift ? L'P' : L'p');}
                if(Monocle::Input::IsKeyPressed(Monocle::KEY_Q)) { m_Canvas->InputCharacter( shift ? L'Q' : L'q');}
                if(Monocle::Input::IsKeyPressed(Monocle::KEY_R)) { m_Canvas->InputCharacter( shift ? L'R' : L'r');}
                if(Monocle::Input::IsKeyPressed(Monocle::KEY_S)) { m_Canvas->InputCharacter( shift ? L'S' : L's');}
                if(Monocle::Input::IsKeyPressed(Monocle::KEY_T)) { m_Canvas->InputCharacter( shift ? L'T' : L't');}
                if(Monocle::Input::IsKeyPressed(Monocle::KEY_U)) { m_Canvas->InputCharacter( shift ? L'U' : L'u');}
                if(Monocle::Input::IsKeyPressed(Monocle::KEY_V)) { m_Canvas->InputCharacter( shift ? L'V' : L'v');}
                if(Monocle::Input::IsKeyPressed(Monocle::KEY_W)) { m_Canvas->InputCharacter( shift ? L'W' : L'w');}
                if(Monocle::Input::IsKeyPressed(Monocle::KEY_X)) { m_Canvas->InputCharacter( shift ? L'X' : L'x');}
                if(Monocle::Input::IsKeyPressed(Monocle::KEY_Y)) { m_Canvas->InputCharacter( shift ? L'Y' : L'y');}
                if(Monocle::Input::IsKeyPressed(Monocle::KEY_Z)) { m_Canvas->InputCharacter( shift ? L'Z' : L'z');}

                if(Monocle::Input::IsKeyPressed(Monocle::KEY_SPACE)) {m_Canvas->InputCharacter( L' ');}
                if(Monocle::Input::IsKeyPressed(Monocle::KEY_EQUALS)) {m_Canvas->InputCharacter( shift ? L'+' : L'=');}
                if(Monocle::Input::IsKeyPressed(Monocle::KEY_SLASH)) { m_Canvas->InputCharacter( shift ? L'?' : L'/');}
                if(Monocle::Input::IsKeyPressed(Monocle::KEY_BACKSLASH)) { m_Canvas->InputCharacter( shift ? L'|' : L'\\');}
                if(Monocle::Input::IsKeyPressed(Monocle::KEY_BACKQUOTE)) { m_Canvas->InputCharacter( shift ? L'~' : L'`');}
                if(Monocle::Input::IsKeyPressed(Monocle::KEY_LEFTBRACKET)) { m_Canvas->InputCharacter( shift ? L'{' : L'[');}
                if(Monocle::Input::IsKeyPressed(Monocle::KEY_RIGHTBRACKET)) { m_Canvas->InputCharacter( shift ? L'}' : L']');}
                if(Monocle::Input::IsKeyPressed(Monocle::KEY_SEMICOLON)) { m_Canvas->InputCharacter( shift ? L':' : L';');}
                if(Monocle::Input::IsKeyPressed(Monocle::KEY_APOSTROPHE)) { m_Canvas->InputCharacter( shift ? L'\"' : L'\''); }
                if(Monocle::Input::IsKeyPressed(Monocle::KEY_PERIOD)) { m_Canvas->InputCharacter( shift ? L'>' : L'.');}
                if(Monocle::Input::IsKeyPressed(Monocle::KEY_COMMA)) { m_Canvas->InputCharacter( shift ? L'<' : L',');}
                if(Monocle::Input::IsKeyPressed(Monocle::KEY_1)) { m_Canvas->InputCharacter( shift ? L'!' : L'1');}
                if(Monocle::Input::IsKeyPressed(Monocle::KEY_2)) { m_Canvas->InputCharacter( shift ? L'@' : L'2');}
                if(Monocle::Input::IsKeyPressed(Monocle::KEY_3)) { m_Canvas->InputCharacter( shift ? L'#' : L'3');}
                if(Monocle::Input::IsKeyPressed(Monocle::KEY_4)) { m_Canvas->InputCharacter( shift ? L'$' : L'4');}
                if(Monocle::Input::IsKeyPressed(Monocle::KEY_5)) { m_Canvas->InputCharacter( shift ? L'%' : L'5');}
                if(Monocle::Input::IsKeyPressed(Monocle::KEY_6)) { m_Canvas->InputCharacter( shift ? L'^' : L'6');}
                if(Monocle::Input::IsKeyPressed(Monocle::KEY_7)) { m_Canvas->InputCharacter( shift ? L'&' : L'7');}
                if(Monocle::Input::IsKeyPressed(Monocle::KEY_8)) { m_Canvas->InputCharacter( shift ? L'*' : L'8');}
                if(Monocle::Input::IsKeyPressed(Monocle::KEY_9)) { m_Canvas->InputCharacter( shift ? L'(' : L'9');}
                if(Monocle::Input::IsKeyPressed(Monocle::KEY_0)) { m_Canvas->InputCharacter( shift ? L')' : L'0');}


                static Monocle::Vector2 lastMPos = Monocle::Vector2(0,0);
                static int lastMScroll = 0;

                Monocle::Vector2 mpos = Monocle::Input::GetMousePosition();
                m_Canvas->InputMouseMoved( mpos.x, mpos.y, mpos.x - lastMPos.x, mpos.y - lastMPos.y );

                static bool lastMouseLeftState = Monocle::Input::IsMouseButtonHeld(Monocle::MOUSE_BUTTON_LEFT),
                            lastMouseRightState = Monocle::Input::IsMouseButtonHeld(Monocle::MOUSE_BUTTON_RIGHT),
                            lastMouseMiddleState = Monocle::Input::IsMouseButtonHeld(Monocle::MOUSE_BUTTON_MIDDLE);

                if(lastMouseLeftState != Monocle::Input::IsMouseButtonHeld(Monocle::MOUSE_BUTTON_LEFT))
                {
                    m_Canvas->InputMouseButton( 0, Monocle::Input::IsMouseButtonHeld(Monocle::MOUSE_BUTTON_LEFT) );
                    lastMouseLeftState = Monocle::Input::IsMouseButtonHeld(Monocle::MOUSE_BUTTON_LEFT);
                }
                if(lastMouseRightState != Monocle::Input::IsMouseButtonHeld(Monocle::MOUSE_BUTTON_RIGHT))
                {
                    m_Canvas->InputMouseButton( 1, Monocle::Input::IsMouseButtonHeld(Monocle::MOUSE_BUTTON_RIGHT) );
                    lastMouseRightState = Monocle::Input::IsMouseButtonHeld(Monocle::MOUSE_BUTTON_RIGHT);
                }
                if(lastMouseMiddleState != Monocle::Input::IsMouseButtonHeld(Monocle::MOUSE_BUTTON_MIDDLE))
                {
                    m_Canvas->InputMouseButton( 2, Monocle::Input::IsMouseButtonHeld(Monocle::MOUSE_BUTTON_MIDDLE) );
                    lastMouseMiddleState = Monocle::Input::IsMouseButtonHeld(Monocle::MOUSE_BUTTON_MIDDLE);
                }

                lastMScroll = Monocle::Input::GetMouseScroll();
                lastMPos = Monocle::Input::GetMousePosition();
            }

        protected:

            Gwen::Controls::Canvas* m_Canvas;

        };
    }
}
