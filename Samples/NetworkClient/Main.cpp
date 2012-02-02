#include "Client.h"
#include <Game.h>
#include <Network/SocketStream.h>
int main()
{
	Monocle::Game g;
	NetworkClient::ClientScene scene;
	g.SetScene(&scene);
	g.Main();
	return 0;
}