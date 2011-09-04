#pragma once

#include "Component.h"

namespace Monocle
{
	class EntityComponent : public Component
	{
	public:
		EntityComponent() : Component() { }
		virtual ~EntityComponent() { }
	};
}