#include "Box2DEntityComponent.h"
#include <Entity.h>
#include <Game.h>
#include "Box2DSceneComponent.h"
#include <Component/Entity/Transform.h>

namespace Monocle
{
	Box2DEntity::Box2DEntity()
		: body(NULL)
	{
		AddDependency("Transform");
	}

	Box2DEntity::~Box2DEntity()
	{
		world->DestroyBody(body);
	}

	void Box2DEntity::Init(Entity *ent)
	{
		EntityComponent::Init(ent);

		Scene *scene = this->GetEntity()->GetScene();
		world = (Box2DSceneComponent*)(*scene)["Box2D"];

		Vector2 pos = GetEntity()->GetComponent<Transform>("Transform")->position;
		bodydef.position = b2Vec2(pos.x, pos.y);

		body = world->CreateBody(bodydef);
	}

	void Box2DEntity::Update()
	{
		Transform *trans = GetEntity()->GetComponent<Transform>("Transform");

		const b2Vec2 pos = body->GetPosition();

		trans->position = Vector2(pos.x, pos.y);
		trans->rotation = body->GetAngle();
	}

	void Box2DEntity::Render()
	{
		
	}

	void Box2DEntity::Unload()
	{

	}

	Box2DEntity *Box2DEntity::Clone() const
	{
		return NULL;
	}

	void Box2DEntity::Save(FileNode *myNode)
	{

	}
	void Box2DEntity::Load(FileNode *myNode)
	{

	}

	void Box2DEntity::LoadBodyDef(b2BodyDef def)
	{
		bodydef = def;
		
		if(world)
		{
			if(body)
			{
				world->DestroyBody(body);
			}
			body = world->CreateBody(bodydef);
		}
	}

	b2Body *Box2DEntity::GetBody()
	{
		return body;
	}
}