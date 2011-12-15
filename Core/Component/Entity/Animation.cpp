#include "Animation.h"
#include "../../Entity.h"

#include "Sprite.h"
#include "Puppet.h"

namespace Monocle
{
	const std::string Animation::ComponentName = "Animation";

	void Animation::Init(Entity *entity)
	{
		EntityComponent::Init(entity);

		if( GetEntity()->HasComponent( MONOCLE_ENTITYCOMPONENT_SPRITE ) )
		{

		}

		if( GetEntity()->HasComponent( MONOCLE_ENTITYCOMPONENT_PUPPET ) )
		{

		}
	}

	void Animation::Update()
	{
		
	}
}