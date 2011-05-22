#include <GUI/gwen/GwenMonocleRenderer.h>
#include <Gwen/Skins/Simple.h>
#include <GUI/gwen/Gwen.h>
#include <Gwen/Controls/Button.h>

#include <Monocle.h>

class GUIScene : public Monocle::Scene
{
public:
    void Begin()
    {
        pRenderer = new Monocle::GUI::GwenRenderer();
        Gwen::Skin::Simple skin;
        skin.SetRender( pRenderer );

        panel = new Monocle::GUI::GwenPanel(&skin);
        panel->SetSize(1024,512);

        pButton = new Gwen::Controls::Button( panel );
        pButton->SetBounds( 10, 10, 200, 100 );
        pButton->SetText( "My First Button" );
    }

    void End()
    {
        delete pButton;
        pButton = NULL;

        delete panel;
        panel = NULL;

        delete pRenderer;
        pRenderer = NULL;
    }
private:
    Monocle::GUI::GwenRenderer *pRenderer;
    Monocle::GUI::GwenPanel *panel;
    Gwen::Controls::Button *pButton;
};

int main()
{
    Monocle::Game *g = new Monocle::Game();
    g->SetScene( new GUIScene() );
    g->Main();
    return 0;
}
