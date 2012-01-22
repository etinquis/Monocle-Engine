#pragma once

#include "Socket.h"

namespace Monocle
{
	class ServerSocket : public Socket
	{
	public:
		ServerSocket();

		void Bind(int port);
		void Listen();
		SocketStream *Accept();
	};
}