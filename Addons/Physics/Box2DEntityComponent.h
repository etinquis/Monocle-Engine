#pragma once

#include <Entity.h>
#include <Component/EntityComponent.h>
#include <set>
#include <string>
#include <Box2D/Box2D.h>

namespace Monocle
{
	class FileNode;
	class Transform;
	class Vector2;
	class Box2DSceneComponent;

	class Box2DEntity : public EntityComponent, public EventHandler<Entity::EventHandler>
	{
	public:
		Box2DEntity();
		virtual ~Box2DEntity();
		
		virtual void Init(Entity *ent);
		void LoadBodyDef(b2BodyDef def);

		void AddedToScene(const Entity::EventHandler::EntityEventArgs& args);
		void Destroyed(const Entity::EventHandler::EntityEventArgs& args);

		void Render();
		void Update();
		void Unload();

		static const std::string ComponentName;
		const std::string& GetName() { return Box2DEntity::ComponentName; }
		b2Body *GetBody();

		Box2DEntity *Clone() const;
	protected:
		void Save(FileNode *myNode);
		void Load(FileNode *myNode);
	private:
		Box2DEntity(const Box2DEntity& tag);

		b2Body *body;
		b2BodyDef bodydef;

		Box2DSceneComponent *world;

		Transform *trans;
	};
}