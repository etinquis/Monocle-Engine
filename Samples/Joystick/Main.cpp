#include "Monocle.h"
#include "Joystick.h"

using namespace Monocle;

int main(void)
{
	Game *game = new Game();
	game->SetScene(new ::Joystick::GameScene());
	game->Main();
	delete game;
	return 0;
}
