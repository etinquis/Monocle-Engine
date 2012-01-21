#pragma once

#include "InputSource.h"
#include "../Vector2.h"
#include <vector>
#include <cmath>
#include "../Debug.h"

namespace Monocle
{
	class Joystick : public InputSource
	{
	public:
		enum JoystickButtons
		{
			JSBUTTON_XB_X = 0x00,
			JSBUTTON_XB_A = 0x01,
			JSBUTTON_XB_B = 0x02,
			JSBUTTON_XB_Y = 0x03,
			JSBUTTON_XB_LBUMPER = 0x04,
			JSBUTTON_XB_RBUMPER = 0x05,
			JSBUTTON_XB_LTRIGGER = 0x06,
			JSBUTTON_XB_RTRIGGER= 0x07,
			JSBUTTON_XB_BACK = 0x08,
			JSBUTTON_XB_START = 0x09,

			JSBUTTON_PS_SQUARE = 0x00,
			JSBUTTON_PS_CROSS = 0x01,
			JSBUTTON_PS_CIRCLE = 0x02,
			JSBUTTON_PS_TRIANGLE = 0x03,
			JSBUTTON_PS_L1 = 0x04,
			JSBUTTON_PS_R1 = 0x05,
			JSBUTTON_PS_L2 = 0x06,
			JSBUTTON_PS_R2 = 0x07,
			JSBUTTON_PS_SELECT = 0x08,
			JSBUTTON_PS_START = 0x09
		};

		class Axis
		{
		public:
			Axis(Vector2 min, Vector2 max, Vector2 deadzone = Vector2(.05, .05))
			{
				this->min = min;
				this->max = max;
				this->deadzone = deadzone;
				this->halfs = Vector2( 
					((max.x - min.x) / 2),
					((max.y - min.y) / 2));
				Debug::Log(halfs.x);
			}
			
			void SetPositionX(int ix)
			{
				float x = (ix - min.x - halfs.x) / (float)halfs.x;
				x = std::abs(x) < deadzone.x ? 0 : x;
				position.x = x;
			}
			void SetPositionY(int iy)
			{
				float y = (iy - min.y - halfs.y) / (float)halfs.y;
				y = std::abs(y) < deadzone.y ? 0 : y;
				position.y = y;
			}
			void SetPosition(int ix, int iy)
			{
				SetPositionX(ix);
				SetPositionY(iy);
			}
			void SetDeadzone(const Vector2& dz)
			{
				deadzone = dz;
			}

			const Vector2& GetPosition()
			{
				return position;
			}
		private:
			Vector2 min, max;
			Vector2 position;
			Vector2 deadzone;
			Vector2 halfs;
		};

		virtual void Update() = 0;
		virtual unsigned char GetButtonCount()
		{
			return ButtonStates.size();
		}
		virtual unsigned char GetAxisCount()
		{
			return Axes.size();
		}

		virtual bool isButtonHeld(unsigned char buttonNum)
		{
			return ButtonStates[buttonNum] && LastButtonStates[buttonNum];
		}
		virtual bool isButtonPressed(unsigned char buttonNum)
		{
			return ButtonStates[buttonNum] && !LastButtonStates[buttonNum];
		}
		virtual bool isButtonReleased(unsigned char buttonNum)
		{
			return !ButtonStates[buttonNum] && LastButtonStates[buttonNum];
		}

		virtual void SetDeadzone(int axis, const Vector2& dz)
		{
			Axes[axis].SetDeadzone(dz);
		}

		Vector2 GetLStick() { return Axes[0].GetPosition(); }
		Vector2 GetRStick() { return Axes[1].GetPosition(); }

		Vector2 GetAxis(int axis) { return Axes[axis].GetPosition(); }

	protected:
		Joystick() { }

		std::vector<bool> ButtonStates;
		std::vector<bool> LastButtonStates;
		std::vector<Axis> Axes;

	private:
		Joystick(const Joystick& js);
	};
}
