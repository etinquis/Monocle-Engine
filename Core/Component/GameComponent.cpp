#include "GameComponent.h"

namespace Monocle
{
	GameComponent::GameComponent() : Component()
	{

	}

	GameComponent::~GameComponent()
	{

	}

	void GameComponent::Init(Game* game)
	{
		this->game = game;
	}
}