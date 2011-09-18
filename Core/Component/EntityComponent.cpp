#include "EntityComponent.h"
#include "../Entity.h"

namespace Monocle
{
	EntityComponent::EntityComponent() : Component()
	{

	}

	EntityComponent::EntityComponent(const EntityComponent &ecp)
		: entity(ecp.entity->Clone())
	{

	}

	EntityComponent::~EntityComponent()
	{

	}

	void EntityComponent::Init(Entity* entity)
	{
		this->entity = entity;
	}

	Entity* EntityComponent::GetEntity()
	{
		return entity;
	}
}