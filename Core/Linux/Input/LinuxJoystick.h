#ifdef MONOCLE_LINUX

#pragma once

#include "../../Input/Joystick.h"

#include <ostream>

namespace Monocle
{
	class LinuxJoystick : public Joystick
	{
	public:
		LinuxJoystick(unsigned int id);
		void Update();

		virtual unsigned char GetButtonCount() { return 1; }
		virtual unsigned char GetAxisCount() { return 1; }

		virtual Vector2 GetAxis1Vector();
		virtual Vector2 GetAxis2Vector();

		virtual bool isButtonHeld(unsigned char buttonNum){return false;}
		virtual bool isButtonPressed(unsigned char buttonNum);
		virtual bool isButtonReleased(unsigned char buttonNum){return false;}

		friend std::ostream &operator<<(std::ostream& os, const LinuxJoystick &js);
	private:
		int joyFileHandle;

		Vector2 Axis1;
		Vector2 Axis2;
	};
}

#endif
