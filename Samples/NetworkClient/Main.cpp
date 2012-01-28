#include "Client.h"
#include <Game.h>

int main()
{
	Monocle::Game g;
	g.SetScene(new NetworkClient::ClientScene());
	g.Main();
	return 0;
}