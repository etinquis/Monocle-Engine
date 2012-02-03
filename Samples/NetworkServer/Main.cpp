#include <Network/ServerSocket.h>
#include <Network/Types/TCP.h>
#include "Server.h"
#include "Network/SocketStream.h"
#include <iostream>

int main()
{
	Monocle::ServerSocket<Monocle::NetworkType::TCP> server;
	server.Bind(8080);
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
