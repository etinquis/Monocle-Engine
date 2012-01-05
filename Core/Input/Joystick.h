#pragma once

#include "InputSource.h"
#include "../Vector2.h"

namespace Monocle
{
	class Joystick : public InputSource
	{
	public:
		virtual void Update() = 0;
		virtual unsigned char GetButtonCount() = 0;
		virtual unsigned char GetAxisCount() = 0;

		virtual bool isButtonHeld(unsigned char buttonNum) = 0;
		virtual bool isButtonPressed(unsigned char buttonNum) = 0;
		virtual bool isButtonReleased(unsigned char buttonNum) = 0;

		virtual Vector2 GetAxis1Vector() = 0;
		virtual Vector2 GetAxis2Vector() = 0;
	};
}