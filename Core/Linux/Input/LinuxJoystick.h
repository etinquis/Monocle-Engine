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

		friend std::ostream &operator<<(std::ostream& os, const LinuxJoystick &js);
	private:
		int joyFileHandle;
	};
}

#endif
