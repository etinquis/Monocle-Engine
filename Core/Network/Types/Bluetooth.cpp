#include "Bluetooth.h"

#ifdef MONOCLE_WINDOWS
#include <ws2bth.h>
#ifndef AF_BLUETOOTH
#define AF_BLUETOOTH AF_BTH
#endif
#endif

namespace Monocle
{
	namespace NetworkType
	{
		SOCKETHANDLE Bluetooth::CreateSocket()
		{
			return socket(AF_BLUETOOTH, SOCK_STREAM, BTHPROTO_RFCOMM);
		}

		void Bluetooth::Bind()
		{

		}
	}
}