#pragma once

#include "Component.h"

namespace Monocle
{
	class Entity;

	class EntityComponent : public Component
	{
	public:
		EntityComponent();
		virtual ~EntityComponent();

		virtual void Init(Entity* entity);
	protected:
		Entity* entity;
	};
}