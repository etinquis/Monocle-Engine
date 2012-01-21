#ifdef MONOCLE_WINDOWS

#pragma once

#include "../../Input/Joystick.h"

//#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <ostream>

namespace Monocle
{
	class WindowsJoystick : public Joystick
	{
	public:
		WindowsJoystick(UINT id);
		void Update();

		virtual unsigned char GetButtonCount() { return 1; }
		virtual unsigned char GetAxisCount() { return 1; }

		virtual Vector2 GetAxis1Vector();
		virtual Vector2 GetAxis2Vector();

		virtual bool isButtonHeld(unsigned char buttonNum){return false;}
		virtual bool isButtonPressed(unsigned char buttonNum);
		virtual bool isButtonReleased(unsigned char buttonNum){return false;}

		friend std::ostream &operator<<(std::ostream& os, const WindowsJoystick &js);
	private:
		UINT uJoyID;
		JOYCAPS caps;
		JOYINFOEX lastInfo;
		JOYINFOEX info;

		Vector2 Axis1;
		Vector2 Axis2;
	};
}

#endif