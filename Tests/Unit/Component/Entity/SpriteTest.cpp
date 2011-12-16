#include "SpriteTest.h"
#include "../Tests.h"

#include <Game.h>

#include <Component/Entity/Sprite.h>
#include <Component/Entity/Transform.h>

#include <cpptest/cpptest-assert.h>

namespace Monocle
{
    namespace Test
    {
        namespace Unit
        {
			class SpriteScene : public Scene
			{
			public:
				void Begin()
				{
					AddEntity<ValidEntity>();
				}
			};

            ValidEntity::ValidEntity()
			{
				AddComponent<Transform>(Transform::InitParams(Vector2(500,500)));
				AddComponent<Sprite>(Sprite::InitParams("Tests/Component/Entity/Sprite.png"));
			}

			InvalidEntity::InvalidEntity()
			{
				AddComponent<Sprite>();
			}
			
			SpriteTest::SpriteTest()
            {
                TEST_ADD(SpriteTest::Init)
            }
            
            void SpriteTest::setup()
            {
				game = new Game();
                valid = new ValidEntity();
				invalid = new InvalidEntity();
            }
            
            void SpriteTest::tear_down()
            {
                delete valid;
				valid = NULL;
				delete invalid;
				invalid = NULL;

				delete game;
				game = NULL;
            }
            
            void SpriteTest::Init()
            {
                TEST_ASSERT( ((Sprite*)(*valid)["Sprite"]) != NULL );
				game->SetScene(new SpriteScene());
				
				Graphics::Set2D(800,600);
				game->Main();
				//game->OneLoop();
				//game->OneLoop();
				//game->OneLoop();
				//TEST_ASSERT( ((Sprite*)(*invalid)["Sprite"]) == NULL );
            }
        }
    }
}
