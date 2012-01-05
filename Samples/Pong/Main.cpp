#include "Monocle.h"
#include "Pong.h"

using namespace Monocle;

int main(void)
{
	Debug::SetLogDestination(Debug::LOG_FILE | Debug::LOG_COUT);
	Game *game = new Game();
	game->SetScene(new Pong::GameScene());
	game->Main();
	delete game;
	return 0;
}
