#include "TestSelector.h"

#include "AudioTest/AudioTest.h"
#include "Flash/Flash.h"
#include "Jumper/Jumper.h"
#include "LevelEditor/LevelEditorTest.h"
#include "Ogmo/Ogmo.h"
#include "Pong/Pong.h"
#include "PuppetTest/PuppetTest.h"
//#include "GUITest/GUITest.h"

#include <Platform.h>
#include <GUI/Berkelium/JSBinding/JSFunctionBinding.h>

namespace TestSelector
{
    LogoPanel::LogoPanel() : Monocle::GUI::BerkeliumPanel()
    {
        SetLayer(-15);
        win->setTransparent(true);

        BindFunction(Monocle::GUI::JSBinding::JSFunctionCallback(&MenuScene::onSelectedTest, L"SelectTest", 1));

        setUrl("http://google.com"); //"file://" + GetWorkingDirectory() + Monocle::Platform::GetDefaultContentPath() + "TestSelector/logooverlay.html");
        win->unfocus();
    }

    void LogoPanel::Update()
    {
        Monocle::GUI::BerkeliumPanel::Update();

        if(Monocle::Input::IsKeyPressed(KEY_ESCAPE))
            setUrl(pagePath); //return to the logo page
    }

    void MenuScene::Begin()
    {
        selectPanel = new Monocle::GUI::BerkeliumPanel();

        selectPanel->BindFunction(Monocle::GUI::JSBinding::JSFunctionCallback(&MenuScene::onSelectedTest, L"SelectTest", 1));

        selectPanel->setUrl("http://google.com"); //"file://" + GetWorkingDirectory() + Assets::GetContentPath() + "TestSelector/selector.html");

        Add(selectPanel);
    }

    void MenuScene::End()
    {
        Scene::End();
    }

    void MenuScene::onSelectedTest(const std::vector<Berkelium::Script::Variant>& args)
    {
        int selectedScene = (int)args[0].toDouble();

        std::cout   << "Argument is:" << selectedScene << std::endl;

        Scene *scene;
        std::string assetPath;

        switch(selectedScene)
        {
        case 1:
            scene = new AudioTest::GameScene();
            break;
        case 2:
            scene = new Flash::TestScene();
            assetPath = "Flash/"; break;
        case 3:
            scene = new Jumper::GameScene();
            assetPath = "Jumper/"; break;
        case 4:
            scene = new Ogmo::World();
            assetPath = "Ogmo/"; break;
        case 5:
            scene = new LevelEditorTest::LevelScene();
            assetPath = "LevelEditorTest/"; break;
        case 6:
            scene = new Pong::GameScene(); break;
        case 7:
            scene = new PuppetTest::TestScene();
            assetPath = "PuppetTest/"; break;
        }

        if(scene) SceneProxy::ChangeScene(scene, assetPath);
    }

    SceneProxy *SceneProxy::instance;
    Scene *SceneProxy::currentScene;
    LogoPanel *SceneProxy::logo;

    SceneProxy::SceneProxy() : Monocle::Scene()
    {
        logo = new LogoPanel();
        Add(logo);
    }

    SceneProxy* SceneProxy::Instance()
    {
        if(!instance) instance = new SceneProxy();
        return instance;
    }

    void SceneProxy::ChangeScene(Scene *scene, std::string assetPath, bool isMenu)
    {
        if(currentScene)
        {
            currentScene->End();
            delete currentScene;
        }

        currentScene = scene;
        Monocle::Assets::SetContentPath(Monocle::Platform::GetDefaultContentPath() + assetPath);
        currentScene->Begin();
        logo->SetSize(Monocle::Vector2(Monocle::Graphics::GetVirtualWidth(), Monocle::Graphics::GetVirtualHeight()));
    }

    void SceneProxy::Begin()
    {
        Scene::Begin();
        if(currentScene)
            currentScene->Begin();
    }

    void SceneProxy::Update()
    {
        Scene::Update();
        if(currentScene)
            currentScene->Update();
    }

    void SceneProxy::Render()
    {
        Monocle::Graphics::PushMatrix();

        if(currentScene)
        {
            currentScene->Render();
        }

        Monocle::Graphics::PopMatrix();
        Scene::Render();
    }
}
