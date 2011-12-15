#include <Monocle.h>
#include <Component/Entity/Sprite.h>
#include <Level/Level.h>

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
	class Player : public Entity
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

	private:
		struct Player_State
		{
			bool jumping;
			bool onGround;
		} state;

		Transform *transform;
		Collidable *collidable;
		Sprite *sprite;
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

	class GameScene : public Scene
	{
	public:
		Player *player;
		Wall *wall;

		void Begin();
		void End();
		void Update();

		void SpawnPlayer(Vector2 pos);
	};
}
