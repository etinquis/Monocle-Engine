#pragma once

#include "Socket.h"
#include "Debug.h"

namespace Monocle
{
	template <typename SocketType>
	class ServerSocket : public Socket<SocketType>
	{
	public:
		ServerSocket() : Socket()
		{

		}

		void Bind(int port)
		{
			struct sockaddr_in addr;

			struct hostent *localhost;
			localhost = gethostbyname("");

			memset(&addr, 0, sizeof(addr));
			addr.sin_family = AF_INET;
			addr.sin_port = htons(port);
			addr.sin_addr.s_addr = INADDR_ANY;

			if( bind(SocketHandle, (struct sockaddr *)&addr, sizeof(addr)) == 0 )
			{
				Debug::Log("Bound socket successfully");
			}
			else
			{
				Close();
				Debug::Log("Could not bind socket");
			}
		}

		void Listen()
		{
			Debug::Log("Socket Listening");
			listen(SocketHandle, 10);
		}

		SocketStream *Accept()
		{
			Debug::Log("Waiting for connection...");
			SOCKETHANDLE streamHandle = accept(SocketHandle, NULL, NULL);

			if(streamHandle >= 0)
			{
				SocketStream *stream = new SocketStream(streamHandle);
				streams.push_back(stream);
				return stream;
			}
			else
			{
				Debug::Log("Connection established");
			}
		}
	};
}