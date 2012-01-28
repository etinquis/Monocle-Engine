#include <Network/ServerSocket.h>
#include <Network/Types/TCP.h>
#include "Server.h"
#include <iostream>

int main()
{
	Monocle::ServerSocket<Monocle::NetworkType::TCP> server;
	server.Bind(8080);
	server.Listen();
	Monocle::SocketStream *stream = server.Accept();

	while(stream->isOpen())
	{
		NetworkServer::KeyStateChangeMessage msg;
		*stream >> msg;
		std::cout << msg.key << std::endl;
	}

	return 0;
}