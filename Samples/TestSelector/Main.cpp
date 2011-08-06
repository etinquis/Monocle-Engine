//
//  Main.cpp
//  MonocleTests
//
//  Created by Josh Whelchel on 5/2/11.
//

#include "Monocle.h"
#include "TestSelector.h"

int main(void)
{
	Monocle::Game *pGame = new Game("Monocle Powered", 1024, 800, 24);
	pGame->SetScene(TestSelector::SceneProxy::Instance());
	pGame->Main();
    delete pGame;
    
	return 0;
}

#endif
