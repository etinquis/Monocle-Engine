//
//  Main.cpp
//  MonocleTests
//
//  Created by Josh Whelchel on 5/2/11.
//

#include "Monocle.h"
#include "AudioTest/AudioTest.h"
#include "Flash/Flash.h"
#include "Jumper/Jumper.h"
#include "LevelEditor/LevelEditorTest.h"
#include "Ogmo/Ogmo.h"
#include "Pong/Pong.h"
#include "PuppetTest/PuppetTest.h"
#include "TestSelector.h"

int main(void)
{
	Monocle::Game *pGame = new Game();
	pGame->SetScene(TestSelector::SceneProxy::Instance());
	pGame->Main();
    delete pGame;
    
	return 0;
}

