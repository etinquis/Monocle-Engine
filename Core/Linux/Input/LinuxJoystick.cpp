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
		int x, y, z, r;

		while( read(joyFileHandle, &e, sizeof( struct js_event )) > 0)
		{
			e.type &= ~JS_EVENT_INIT;
			if( e.type == JS_EVENT_AXIS )
			{
				switch(e.number)
				{
				case 0:
					x = e.value; break;
				case 1:
					y = e.value; break;
				case 2:
					z = e.value; break;
				case 3:
					r = e.value; break;
				}
			}
		}

		Axes[0].SetPosition(x,y);
		Axes[1].SetPosition(z,r);

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
