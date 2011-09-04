#include "EntityComponent.h"

namespace Monocle
{
	EntityComponent::EntityComponent() : Component()
	{

	}

	EntityComponent::~EntityComponent()
	{

	}

	void EntityComponent::Init(Entity* entity)
	{
		this->entity = entity;
	}
}