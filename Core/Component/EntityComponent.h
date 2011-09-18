#pragma once

#include "Component.h"

namespace Monocle
{
	class Entity;

	class EntityComponent : public Component
	{
	public:
		EntityComponent();
		EntityComponent(const EntityComponent &ecp);
		virtual ~EntityComponent();

		virtual void Init(Entity* entity);
		Entity* GetEntity();
	protected:
		Entity* entity;
	};
}