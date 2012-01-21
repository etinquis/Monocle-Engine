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
		Axis1 = Vector2::zero;
		Axis1 = Vector2::zero;
		joyFileHandle = open("/dev/input/js0", O_NONBLOCK);
	}

	void LinuxJoystick::Update()
	{
		struct js_event e;
		while( read(joyFileHandle, &e, sizeof( struct js_event )) > 0)
		{
			Debug::Log("Event");
			e.type &= ~JS_EVENT_INIT;
			if( e.type == JS_EVENT_AXIS )
			{
				if(e.number == 0)
				{
					Axis1.x = e.value;
				}
				else if(e.number == 1)
				{
					Axis1.y = e.value;
				}	
				else if(e.number == 3)
				{
					Axis2.x = e.value;
				}
				else if(e.number == 4)
				{
					Axis2.y = e.value;
				}
			}
		}
		if(errno != EAGAIN)
		{
			Debug::Log("Joystick Error");
			Debug::Log(strerror(errno));
		}

		//Axis1 = Vector2( (xScale - 500) / 500.f , (yScale - 500) / 500.f );
		//Axis2 = Vector2( (zScale - 500) / 500.f , (rScale - 500) / 500.f );
	}

	Vector2 LinuxJoystick::GetAxis1Vector()
	{
		return Axis1;
	}

	Vector2 LinuxJoystick::GetAxis2Vector()
	{
		return Axis2;
	}

	bool LinuxJoystick::isButtonPressed(unsigned char buttonNum)
	{
		return false;
	}

	std::ostream &operator<<(std::ostream& os, const Monocle::LinuxJoystick &js)
	{
		return os;
	}
}


#endif
