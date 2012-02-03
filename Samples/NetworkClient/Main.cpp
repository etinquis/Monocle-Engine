#include "Client.h"
#include <Game.h>
#include <Network/SocketStream.h>
#include <Network/ClientSocket.h>
#include <Network/Types/TCP.h>
int main(int argc, char **argv)
{
	Monocle::Game g;

	std::string host;
	int port;

	if(argc > 1)
	{
		//host
		host.assign(argv[1]);

		if(argc > 2)
		{
			//port
			std::stringstream tmp;
			tmp << argv[2];
			tmp >> port;
		}
	}
	else
	{
		host = "127.0.0.1";
		port = 8080;
	}

	Monocle::ClientSocket<Monocle::NetworkType::TCP> sock;

	Monocle::SocketStream *stream = sock.Connect(port, host);

	if(stream != NULL)
	{
		NetworkClient::ClientScene scene(stream);
		g.SetScene(&scene);
		g.Main();
	}
	return 0;
}