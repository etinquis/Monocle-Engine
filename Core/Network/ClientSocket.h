#pragma once

#include "Socket.h"
#include "Debug.h"
#ifdef MONOCLE_LINUX
	#include <arpa/inet.h>
	#include <netdb.h>
#endif

namespace Monocle
{
	//! Client version of the Socket class, allowing a client application
	//! to connect to a local or remote ServerSocket and send data.
	template <typename SocketType>
	class ClientSocket : public Socket<SocketType>
	{
	public:
		//! Returns a new SocketStream allowing communication with a socket
		//! on the given host and port.  Only one SocketStream can be active
		//! on a ClientSocket, so subsequent calls to Connect() will close
		//! the existing SocketStream if it exists.
		SocketStream *Connect(unsigned short port, std::string host)
		{
			while(this->streams.size() > 0)
			{
				delete (*this->streams.back());
				this->streams.pop_back();
			}

			unsigned long address = inet_addr(host.c_str());

			if(address == INADDR_NONE)
			{
				Debug::Log("Address could not be found");
				return NULL;
			}

			hostent *hostIp = gethostbyaddr((char*) &address, sizeof(address), AF_INET);
			port = htons(port);

			struct sockaddr_in addr;
			addr.sin_family = PF_INET;
			addr.sin_port = port;
			addr.sin_addr.s_addr = *((unsigned long*)hostIp->h_addr);

			if(connect(this->SocketHandle, (struct sockaddr *)&addr, sizeof(addr)) == 0)
			{
				SocketStream *stream = new SocketStream(this->SocketHandle, true);
				this->streams.push_back(stream);
				return stream;
			}
			else
			{
				Debug::Log("Connection failed");
				Close();
				return NULL;
			}
		}

	protected:
		struct hostent *hostIp;
	};
}
