#include <GUI/gwen/GwenMonocleRenderer.h>
#include <Gwen/Skins/Simple.h>
#include <Gwen/Skins/TexturedBase.h>
#include <GUI/gwen/Gwen.h>
#include <Gwen/Controls/Button.h>
#include <Gwen/Controls/HSVColorPicker.h>
#include <Gwen/Controls/TextBox.h>
#include <Gwen/Controls/WindowControl.h>
#include <GUI/gwen/GwenMonocleInput.h>

#include <Monocle.h>

class Skin : public Gwen::Skin::Simple
{
public:
    Skin() : Gwen::Skin::Simple()
    {
        m_DefaultFont.facename = L"AudioTest/LiberationSans-Regular.ttf";
        m_DefaultFont.size = 24;
    }
};

class TexSkin : public Gwen::Skin::TexturedBase
{
public:
    TexSkin() : Gwen::Skin::TexturedBase()
    {

    }

    void Init( Gwen::TextObject texture )
    {
        Gwen::Skin::TexturedBase::Init(texture);
        m_DefaultFont.facename = L"AudioTest/LiberationSans-Regular.ttf";
        m_DefaultFont.size = 24;
    }
};

class GUIScene : public Monocle::Scene
{
public:
    void Begin()
    {
        pRenderer = new Monocle::GUI::GwenRenderer();
        skin = TexSkin();
        skin.SetRender( pRenderer );
        skin.Init("GUITest/DefaultSkin.png");

        panel = new Monocle::GUI::GwenPanel(&skin);
        panel->SetSize(800,600);

        input = Monocle::GUI::GwenInput();
        input.Initialize(panel);

        pButton = new Gwen::Controls::Button( panel );
        pButton->SetBounds(10,10,200,100);
        pButton->SetText( "My First Button" );

        pColor = new Gwen::Controls::HSVColorPicker( panel );
        pColor->SetBounds(10,250,200,100);

        pText = new Gwen::Controls::TextBox( panel );
        pText->SetBounds(250,10,200,100);

        pWindow = new Gwen::Controls::WindowControl( panel );
        pWindow->SetBounds(50,50,200,100);

        Add(panel);
    }

    void Update()
    {
        Monocle::Scene::Update();

        input.ProcessInput();
    }

    void End()
    {
        delete pButton;
        pButton = NULL;

        delete panel;
        panel = NULL;

        delete pRenderer;
        pRenderer = NULL;

        delete pWindow;
        pWindow = NULL;
    }
private:
    Monocle::GUI::GwenRenderer *pRenderer;
    Monocle::GUI::GwenPanel *panel;
    Gwen::Controls::Button *pButton;
    Gwen::Controls::HSVColorPicker *pColor;
    Gwen::Controls::TextBox *pText;
    Gwen::Controls::WindowControl *pWindow;
    Monocle::GUI::GwenInput input;

    TexSkin skin;
};

int main()
{
    Monocle::Game *g = new Monocle::Game();
    g->SetScene( new GUIScene() );
    g->Main();
    return 0;
}
