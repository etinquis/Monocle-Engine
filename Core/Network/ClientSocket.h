#pragma once

#include "Socket.h"
#include "SocketStream.h"
#include "Debug.h"
#ifdef MONOCLE_LINUX
	#include <arpa/inet.h>
	#include <netdb.h>
#elif defined(MONOCLE_WINDOWS)
	#include <ws2tcpip.h>
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

			struct addrinfo *info = NULL;

			int error;

			std::stringstream ss;
			ss << port;
			error = getaddrinfo(host.c_str(), ss.str().c_str(), NULL, &info);

			sockaddr addr;
			int addrLen = this->sType.FindAddr(info, addr);

			if(addrLen == -1)
			{
				Debug::Log("Could not find suitable address.");
				return NULL;
			}

			if(connect(this->SocketHandle, &addr, addrLen) == 0)
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
