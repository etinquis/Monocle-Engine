#pragma once

#include "Component.h"

namespace Monocle
{
	class Game;

	class GameComponent : public Component
	{
	public:
		GameComponent();
		virtual ~GameComponent();

		virtual void Init(Game* game);
	protected:
		Game* game;
	};
}