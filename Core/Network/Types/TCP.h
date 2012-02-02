#pragma once

#include "../Socket.h"

namespace Monocle
{
	namespace NetworkType
	{
		class TCP : public SocketTypeBase
		{
		public:
			virtual SOCKETHANDLE CreateSocket();
			int FindAddr(const addrinfo *addr, sockaddr &result);
		};
	}
}