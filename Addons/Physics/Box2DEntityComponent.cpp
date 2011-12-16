#include "Box2DEntityComponent.h"
#include <Entity.h>
#include <Game.h>
#include "Box2DSceneComponent.h"
#include <Component/Entity/Transform.h>

namespace Monocle
{
	const std::string Box2DEntity::ComponentName = "Box2DEntity";

	Box2DEntity::Box2DEntity()
		: body(NULL)
	{
		AddDependency<Transform>();
	}

	Box2DEntity::~Box2DEntity()
	{
		
	}

	void Box2DEntity::Init(Entity *ent)
	{
		EntityComponent::Init(ent);
	}

	void Box2DEntity::AddedToScene(const Entity::EventHandler::EntityEventArgs& args)
	{
		Scene *scene = entity->GetScene();
		world = scene->GetComponent<Box2DSceneComponent>();

		trans = GetEntity()->GetComponent<Transform>();
		Vector2 pos = trans->position;
		bodydef.position = b2Vec2(pos.x, pos.y);

		body = world->CreateBody(bodydef);
	}

	void Box2DEntity::Destroyed(const Entity::EventHandler::EntityEventArgs& args)
	{
		world->DestroyBody(body);
	}

	void Box2DEntity::Update()
	{
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