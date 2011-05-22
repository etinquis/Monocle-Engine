#include <GUI/gwen/GwenMonocleRenderer.h>
#include <Gwen/Skins/Simple.h>
#include <GUI/Panel.h>

namespace Monocle
{
    namespace GUI
    {
        class GwenPanel : public Panel, public Gwen::Controls::Canvas
        {
        public:
            GwenPanel(Gwen::Skin::Base *pSkin);
            ~GwenPanel();
            void Update();
            void Render();
        };
    }
}
