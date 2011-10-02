#include "Box2D.h"
#include <Game.h>
#include <Physics/Box2DSceneComponent.h>
#include <Physics/Box2DEntityComponent.h>
#include <Component/Entity/Transform.h>

namespace Monocle
{
    namespace Test
    {
        namespace Unit
		{
			class Box : public Entity
			{
			public:
				Box()
				{
					Entity();
					AddComponent<Transform>();

					transform = GetComponent<Transform>("Transform");

					transform->position = Vector2(400, 300);
				}

				void Added()
				{
					box = AddComponent<Box2DEntity>();

					b2BodyDef def;
					def.type = b2_dynamicBody;
					def.position = b2Vec2(transform->position.x, transform->position.y);
					box->LoadBodyDef(def);

					b2PolygonShape dynamicbox;
					dynamicbox.SetAsBox(1,1);

					b2FixtureDef fixturedef;
					fixturedef.shape = &dynamicbox;
					fixturedef.density = 1.0f;
					fixturedef.friction = 0.3f;
					
					box->GetBody()->CreateFixture(&dynamicbox, 1);
				}

				void Update()
				{
					Entity::Update();
					std::cout << transform->position << std::endl;
				}

				Transform *transform;
				Box2DEntity *box;
			};

			class BoxScene : public Scene
			{
			public:
				BoxScene()
				{
					Scene();
					AddComponent<Box2DSceneComponent>();

					GetComponent<Box2DSceneComponent>("Box2D")->SetGravity(Vector2(0,-9.8));
				}

				void Begin()
				{
					Scene::Begin();
					Add(new Box());
				}
			};

			Box2D::Box2D()
			{
				TEST_ADD(Box2D::Simulate);
			}

			void Box2D::setup()
			{

			}

			void Box2D::tear_down()
			{

			}

			void Box2D::Simulate()
			{
				Game *game = new Game();

				game->SetScene(new BoxScene());

				for (int32 i = 0; i < 150; ++i)
				{
					game->OneLoop();
				}
			}

        }
    }
}
