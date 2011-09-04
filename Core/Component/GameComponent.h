#pragma once

#include "Component.h"

namespace Monocle
{
	class GameComponent : public Component
	{
	public:
		GameComponent() : Component() { }
		virtual ~GameComponent() { }
	};
}