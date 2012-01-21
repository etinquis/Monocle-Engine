#include <Monocle.h>
#include <TextureAsset.h>

using namespace Monocle;

namespace Joystick
{
	class JoystickAxis : public Entity
	{
	public:
		JoystickAxis(const Vector2& pos, int axisId,Monocle::Joystick *js);
		void Render();
		void Update();
	private:
		Vector2 basePos;
		Monocle::Joystick *js;
		TextureAsset *stick;
		int axis;
	};

	class JoystickButton : public Entity
	{
	public:
		JoystickButton(const Vector2& pos, int btnId, Monocle::Joystick *js);
		void Render();
		void Update();
	private:
		Monocle::Joystick *js;
		TextureAsset *btn;
		int button;
		bool pressed;
	};

	class Joystick : public Entity
	{
	public:
		Joystick();
		void Render();
		void Update();

	private:
		Sprite *sprite;
		JoystickAxis *LStick;
		JoystickAxis *RStick;

		JoystickButton *Square;
		JoystickButton *Triangle;
		JoystickButton *Cross;
		JoystickButton *Circle;

		JoystickButton *Start;
		JoystickButton *Select;

		Monocle::Joystick *js;
	};

	class GameScene : public Scene
	{
	public:
		void Begin();
		void End();

	private:
		Joystick *js;
	};
}
