#include "Client.h"
#include <Game.h>

int main()
{
	Monocle::Game g;
	NetworkClient::ClientScene scene;
	g.SetScene(&scene);
	g.Main();
	return 0;
}