#pragma once

#include "Socket.h"
#include "Debug.h"

namespace Monocle
{
	template <typename SocketType>
	class ClientSocket : public Socket<SocketType>
	{
	public:
		SocketStream *Connect(unsigned short port, std::string host)
		{
			unsigned long address = inet_addr(host.c_str());

			if(address == INADDR_NONE)
			{
				Debug::Log("Address could not be found");
			}

			hostent *hostIp = gethostbyaddr((char*) &address, sizeof(address), AF_INET);
			port = htons(port);

			struct sockaddr_in addr;
			addr.sin_family = PF_INET;
			addr.sin_port = port;
			addr.sin_addr.s_addr = *((unsigned long*)hostIp->h_addr);

			Debug::Log(WSAGetLastError());
			if(connect(SocketHandle, (struct sockaddr *)&addr, sizeof(addr)) == 0)
			{
				SocketStream *stream = new SocketStream(SocketHandle);
				streams.push_back(stream);
				return stream;
			}
			else
			{
				Debug::Log("Connection failed");
				return NULL;
			}
		}

	protected:
		struct hostent *hostIp;
	};
}