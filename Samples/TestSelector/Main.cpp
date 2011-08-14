//
//  Main.cpp
//  MonocleTests
//
//  Created by Josh Whelchel on 5/2/11.
//

#include "Monocle.h"
#include "TestSelector.h"
#include "LevelEditorTest.h"

int main(void)
{
	Monocle::Game *pGame = new Monocle::Game("Monocle Powered", 1024, 800, 24);
	//Monocle::Assets::SetContentPath(Monocle::Assets::GetContentPath() + "LevelEditorTest/");
	pGame->SetScene(TestSelector::SceneProxy::Instance());
	pGame->Main();
    delete pGame;
    
	return 0;
}
