#ifdef MONOCLE_WINDOWS

#include <windows.h>
#include "WindowsJoystick.h"
#include "../../Debug.h"
#include <iostream>

namespace Monocle
{
	WindowsJoystick::WindowsJoystick(UINT id) : uJoyID(id)
	{
		MMRESULT res = joyGetDevCaps(id, &caps, sizeof(JOYCAPS));

		switch(res)
		{
		case MMSYSERR_NODRIVER:
			Monocle::Debug::Log("No driver found for Joystick #" + id);
		case MMSYSERR_INVALPARAM:
			Monocle::Debug::Log("Invalid param for initialization of Joystick #" + id);
		}
	}

	void WindowsJoystick::Update()
	{
		lastInfo = info;

		JOYINFOEX inf;
		inf.dwFlags = JOY_RETURNALL;
		inf.dwSize = sizeof(JOYINFOEX);
		MMRESULT res = joyGetPosEx(uJoyID, &inf);

		switch(res)
		{
		case MMSYSERR_NODRIVER:
			Debug::Log("The joystick driver is not present");
			break;
		case MMSYSERR_INVALPARAM:
			Debug::Log("An invalid parameter was passed.");
			break;
		case MMSYSERR_BADDEVICEID:
			Debug::Log("The specified joystick identifier is invalid.");
			break;
		case JOYERR_UNPLUGGED:
			Debug::Log("The specified joystick is not connected to the system.");
			break;
		case JOYERR_PARMS:
			Debug::Log("The specified joystick identifier is invalid.");
			break;
		}

		info = inf;

		int xScale = floor( (info.dwXpos - caps.wXmin) / (double)(caps.wXmax - caps.wXmin) * 1000 + 0.5 );
		int yScale = floor( (info.dwYpos - caps.wYmin) / (double)(caps.wYmax - caps.wYmin) * 1000 + 0.5 );
		int zScale = floor( (info.dwZpos - caps.wZmin) / (double)(caps.wZmax - caps.wZmin) * 1000 + 0.5 );
		int rScale = floor( (info.dwRpos - caps.wRmin) / (double)(caps.wRmax - caps.wRmin) * 1000 + 0.5 );

		Axis1 = Vector2( (xScale - 500) / 500.f , (yScale - 500) / 500.f );
		Axis2 = Vector2( (zScale - 500) / 500.f , (rScale - 500) / 500.f );
	}

	Vector2 WindowsJoystick::GetAxis1Vector()
	{
		return Axis1;
	}

	Vector2 WindowsJoystick::GetAxis2Vector()
	{
		return Axis2;
	}

	bool WindowsJoystick::isButtonPressed(unsigned char buttonNum)
	{
		return !(lastInfo.dwButtons & (1 << (buttonNum - 1))) && (info.dwButtons & (1 << (buttonNum - 1)));
	}

	std::ostream &operator<<(std::ostream& os, const Monocle::WindowsJoystick &js)
	{
		os << "Joystick: {"
				<< "id: " << js.uJoyID
				<< "joycap: {"
					<< "name: " << js.caps.szPname
				<< "}"
			<< "}" << std::endl;
		return os;
	}
}


#endif