#include "TCP.h"
#include <cstring>

#ifdef MONOCLE_WINDOWS
	#include <ws2tcpip.h>
#endif

#include "../SocketStream.h"

namespace Monocle
{
	namespace NetworkType
	{
		SOCKETHANDLE TCP::CreateSocket()
		{
			return socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		}

		int TCP::FindAddr(const addrinfo *addr, sockaddr &result)
		{
			memset(&result, 0, sizeof(result));

			if(addr == NULL)
			{
				return -1;
			}
			
			for(const addrinfo *current = addr; current != NULL; current = current->ai_next)
			{
				switch(current->ai_family)
				{
				case AF_INET:
				{
					result = *current->ai_addr;
					return sizeof(struct sockaddr_in);
				}
				case AF_INET6:
				{
					result = *current->ai_addr;
					return sizeof(struct sockaddr_in6);
				}
				default:
					break;
				}
			}

			return -1;
		}
	}
}
