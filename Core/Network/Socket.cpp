#include "Socket.h"
#include "Network.h"

namespace Monocle
{
	SocketInit::SocketInit()
	{
	#ifdef MONOCLE_WINDOWS
		WSADATA data;

		int res;
		res = WSAStartup(MAKEWORD(2,2), &data);

		switch(res)
		{
		case 0:
			SocketsAvailable = true; break;
		default:
			SocketsAvailable = false; break;
		}
	#endif
	}

	SocketInit::~SocketInit()
	{
	#ifdef MONOCLE_WINDOWS
		if(SocketsAvailable)
		{
			WSACleanup();
		}
	#endif
	}
}