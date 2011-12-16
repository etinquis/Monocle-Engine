#include "EntityComponent.h"
#include "../Entity.h"
#include "../File/FileNode.h"

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

	void EntityComponent::Render()
	{

	}

	Entity* EntityComponent::GetEntity()
	{
		return entity;
	}

	void EntityComponent::SaveTo(FileNode *parentNode)
	{
		Save(parentNode->InsertEndChildNode(GetName()));
	}

	void EntityComponent::LoadFrom(FileNode *parentNode)
	{
		FileNode *myNode = parentNode->GetChild(GetName());
		if(myNode)
		{
			Load( myNode );
		}
	}
}