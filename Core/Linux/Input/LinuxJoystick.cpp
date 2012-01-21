#ifdef MONOCLE_LINUX

#include "LinuxJoystick.h"
#include "../../Debug.h"
#include <iostream>
#include <linux/joystick.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

namespace Monocle
{
	LinuxJoystick::LinuxJoystick(unsigned int id) 
	{
		joyFileHandle = open("/dev/input/js0", O_NONBLOCK);

		char out;
		ioctl( joyFileHandle, JSIOCGAXES, &out );

		for(int i = 0; i < out; i++)
		{
			Axes.push_back(
				Axis(
					Vector2(-32767, -32767), 
					Vector2(32767, 32767)
				)
			);
		}

		ioctl( joyFileHandle, JSIOCGBUTTONS, &out );

		for(int i = 0; i < out; i++)
		{
			ButtonStates.push_back(false);
		}
	}

	void LinuxJoystick::Update()
	{
		struct js_event e;
		LastButtonStates = ButtonStates;
		while( read(joyFileHandle, &e, sizeof( struct js_event )) > 0)
		{
			e.type &= ~JS_EVENT_INIT;
			if( e.type == JS_EVENT_AXIS )
			{
				switch(e.number)
				{
				case 0:
					Axes[0].SetPositionX(e.value); break;
				case 1:
					Axes[0].SetPositionY(e.value); break;
				case 2:
					Axes[1].SetPositionX(e.value); break;
				case 3:
					Axes[1].SetPositionY(e.value); break;
				}
			}
			else if( e.type == JS_EVENT_BUTTON )
			{
				ButtonStates[e.number] = e.value;
			}
		}
		if(errno != EAGAIN)
		{
			Debug::Log("Joystick Error");
			Debug::Log(strerror(errno));
		}
	}

	std::ostream &operator<<(std::ostream& os, const Monocle::LinuxJoystick &js)
	{
		return os;
	}
}


#endif
