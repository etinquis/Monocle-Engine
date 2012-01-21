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

		friend std::ostream &operator<<(std::ostream& os, const WindowsJoystick &js);
	private:
		UINT uJoyID;
		JOYCAPS caps;
		JOYINFOEX lastInfo;
		JOYINFOEX info;
	};
}

#endif