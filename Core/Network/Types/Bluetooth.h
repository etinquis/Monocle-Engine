#pragma once

#include "../Socket.h"
#include "../Network.h"

namespace Monocle
{
	class ServerSocket;

	namespace NetworkType
	{
		class Bluetooth : public SocketTypeBase
		{
		public:
			virtual SOCKETHANDLE CreateSocket();
			virtual SocketStream *Connect(const sockaddr_in *addr);
		};
	}
}