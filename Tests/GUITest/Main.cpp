#include <GUI/gwen/GwenMonocleRenderer.h>
#include <Gwen/Skins/Simple.h>
#include <Gwen/Skins/TexturedBase.h>
#include <GUI/gwen/Gwen.h>
#include <Gwen/Controls/Button.h>
#include <Gwen/Controls/HSVColorPicker.h>
#include <Gwen/Controls/TextBox.h>
#include <Gwen/Controls/WindowControl.h>
#include <GUI/gwen/GwenMonocleInput.h>

#include <Gwen/UnitTest/UnitTest.h>

#include <Monocle.h>

class Skin : public Gwen::Skin::Simple
{
public:
    Skin() : Gwen::Skin::Simple()
    {
        m_DefaultFont.facename = L"AudioTest/LiberationSans-Regular.ttf";
        m_DefaultFont.size = 12;
        
        m_MarlettFont.facename = L"AudioTest/LiberationSans-Regular.ttf";
        m_MarlettFont.size = 12;
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
        m_DefaultFont.size = 12;
        
        m_MarlettFont.facename = L"AudioTest/LiberationSans-Regular.ttf";
        m_MarlettFont.size = 12;
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

        UnitTest *pUnit = new UnitTest( panel );
        
        input.Initialize( panel );

        Add(panel);
    }

    void Update()
    {
        Monocle::Scene::Update();

        input.ProcessInput();
    }

    void End()
    {
        delete panel;
        panel = NULL;

        delete pRenderer;
        pRenderer = NULL;

    }
private:
    Monocle::GUI::GwenRenderer *pRenderer;
    Monocle::GUI::GwenPanel *panel;
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
