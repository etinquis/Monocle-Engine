#include <Monocle.h>
#include <Component/Entity/Sprite.h>
#include <Level/Level.h>
#include <Input.h>

// by @NoelFB
using namespace Monocle;

namespace Monocle
{
	class Transform;
	class Collidable;
	class Sprite;
}

namespace Jumper
{
	class Player : public Entity, public EventHandler<Input::EventHandler>
	{
	public:
		Player(Vector2 pos);
		void Update();
		float Sign(float i, float to);

		Vector2 velocity;
		float gravity;
		float speed;
		float jump;
		float maxSpeed;

		float leanAmount; // how much does he lean into his left/right movement?
		
		Transform *transform;
		Collidable *collidable;
		Sprite *sprite;
	private:
		struct Player_State
		{
			bool jumping;
			bool onGround;
		} state;
	};

	
	class Wall : public Entity
	{
	public:
		Wall(Vector2 p, float w, float h);
		//void Update();
		void Render();

		//temporary, replace base class with Sprite eventually
		float width, height;

	private:
		Transform *transform;
		Collidable *collidable;
	};

	class GameScene : public Scene, public EventHandler<Input::EventHandler>
	{
	public:
		Player *player;
		Wall *wall;

		GameScene();

		void Begin();
		void End();
		void Update();

		void OnKeyPress(KeyCode key);
		void OnMousePress(const Input::EventHandler::MouseButtonEventArgs& args);

		void SpawnPlayer(Vector2 pos);
	};
}
