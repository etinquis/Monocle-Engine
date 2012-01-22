#include "ServerSocket.h"

namespace Monocle
{
	ServerSocket::ServerSocket() : Socket()
	{

	}

	void ServerSocket::Bind(int port)
	{
		struct sockaddr_in addr;

		struct hostent *localhost;
		localhost = gethostbyname("");

		memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		addr.sin_addr.s_addr = INADDR_ANY;

		bind(SocketHandle, (struct sockaddr *)&addr, sizeof(addr));
	}

	void ServerSocket::Listen()
	{
		listen(SocketHandle, 10);
	}

	void ServerSocket::Accept()
	{

	}
}