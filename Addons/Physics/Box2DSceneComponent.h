#pragma once

#include <Component/SceneComponent.h>
#include <set>
#include <string>
#include <Box2D/Box2D.h>

#define MONOCLE_GAMECOMPONENT_BOX2D "Box2D"

namespace Monocle
{
	class Entity;
	class FileNode;
	class Vector2;

	class Box2DSceneComponent : public SceneComponent
	{
	public:
		Box2DSceneComponent();
		virtual ~Box2DSceneComponent();

		void Update();
		void Unload();

		void SetGravity(Vector2 grav);

		std::string GetName() { return MONOCLE_GAMECOMPONENT_BOX2D; }
		b2Body *CreateBody(b2BodyDef &bodydef);
		void DestroyBody(b2Body *body);

		Box2DSceneComponent *Clone() const;
	protected:
		void Save(FileNode *myNode);
		void Load(FileNode *myNode);
	private:
		Box2DSceneComponent(const Box2DSceneComponent& b2d);

		b2World world;
		b2Vec2 gravity;
	};
}