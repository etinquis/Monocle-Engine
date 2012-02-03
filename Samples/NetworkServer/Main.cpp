#include <Network/ServerSocket.h>
#include <Network/Types/TCP.h>
#include "Server.h"
#include "Network/SocketStream.h"
#include <iostream>

int main(int argc, char **argv)
{
	int port = 8080;

	if(argc > 1)
	{
		std::stringstream tmp;
		tmp << argv[1];
		tmp >> port;
	}

	Monocle::ServerSocket<Monocle::NetworkType::TCP> server;
	server.Bind(port);
	server.Listen();
	Monocle::SocketStream *stream = server.Accept();

	if(stream != NULL)
	{
		NetworkServer::KeyStateChangeMessage msg;
		while( (*stream >> msg).good() )
		{
			std::cout << msg.key << std::endl;
		}
	}
	return 0;
}
