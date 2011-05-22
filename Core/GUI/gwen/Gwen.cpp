#include <GUI/gwen/Gwen.h>

namespace Monocle
{
    namespace GUI
    {
        GwenPanel::GwenPanel( Gwen::Skin::Base* skin ) : Gwen::Controls::Canvas( skin ), Panel()
        {

        }

        GwenPanel::~GwenPanel() {}

        void GwenPanel::Update()
        {
            DoThink();
        }

        void GwenPanel::Render()
        {
            this->RenderCanvas();
        }
    }
}
