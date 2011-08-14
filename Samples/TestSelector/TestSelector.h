//  Main.cpp
//  MonocleTests
//
//  Created by Josh Whelchel on 5/2/11.
//

#include <Scene.h>
#include <GUI/Berkelium/BerkeliumPanel.h>

namespace Monocle
{
    class Vector2;
}

namespace TestSelector
{
    class LogoPanel : public Monocle::GUI::BerkeliumPanel
    {
    public:
        LogoPanel();
        void Update();
        
        static void onSelectedTest(const std::vector<Berkelium::Script::Variant>& args);
    private:
        std::string pagePath;
    };

    class SceneProxy : public Monocle::Scene
    {
    public:
        static SceneProxy *Instance();
        static void ChangeScene(Scene *scene, std::string assetPath, bool isMenu = false);
		void Begin();
        void Update();
        void Render();
    private:
        SceneProxy();

        static void SetScene(Scene *scene);
        static Scene *currentScene;
        static SceneProxy *instance;

        static LogoPanel *logo;
    };
};
