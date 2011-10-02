#pragma once

#include <Component/EntityComponent.h>
#include <set>
#include <string>
#include <Box2D/Box2D.h>

#define MONOCLE_ENTITYCOMPONENT_BOX2D "Box2D"

namespace Monocle
{
	class FileNode;
	class Vector2;
	class Box2DSceneComponent;

	class Box2DEntity : public EntityComponent
	{
	public:
		Box2DEntity();
		virtual ~Box2DEntity();
		
		virtual void Init(Entity *ent);
		void LoadBodyDef(b2BodyDef def);

		void Render();
		void Update();
		void Unload();

		std::string GetName() { return MONOCLE_ENTITYCOMPONENT_BOX2D; }
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
	};
}