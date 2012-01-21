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

		if(caps.wNumAxes > 0)
		{
			Axes.push_back(
				Axis(
					Vector2(caps.wXmin, caps.wYmin), 
					Vector2(caps.wXmax, caps.wYmax)
				)
			);
		}
		if(caps.wNumAxes > 1)
		{
			Axes.push_back(
				Axis(
					Vector2(caps.wZmin, caps.wRmin), 
					Vector2(caps.wZmax, caps.wRmax)
				)
			);
		}
		for(int i = 0; i < caps.wNumButtons; i++)
		{
			ButtonStates.push_back(false);
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

		Axes[0].SetPosition( info.dwXpos, info.dwYpos );
		Axes[1].SetPosition( info.dwZpos, info.dwRpos );

		int button = 0;
		LastButtonStates = ButtonStates;
		for(std::vector<bool>::iterator it = ButtonStates.begin(); it != ButtonStates.end(); it++, button++)
		{
			ButtonStates[button] = (info.dwButtons & (1 << (button)));
		}
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