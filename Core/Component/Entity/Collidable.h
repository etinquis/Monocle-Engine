#pragma once

#include "../EntityComponent.h"
#include "Tag.h"
#include <set>
#include <string>

#define MONOCLE_ENTITYCOMPONENT_COLLIDABLE "Collidable"

namespace Monocle
{
	class Collider;
	class CollisionData;
	class Vector2;

	class Collidable : public Tag
	{
	public:
		typedef struct CollidableInitParams
		{
		public:
			std::string tag;
		} InitParams;

		Collidable();
		Collidable(const Collidable& tag);
		virtual ~Collidable();

		void Update();
		void Unload();
		
		static const std::string ComponentName;
		const std::string& GetName() { return Collidable::ComponentName; }

		//! Check our collider against all entities that have "tag"
		Collider* Collide(const std::string &tag, CollisionData *collisionData=NULL);
		//! Check our collider against all entities that have "tag" - warping us to atPosition first, then back to our original position after
		Collider* CollideAt(const std::string &tag, const Vector2& atPosition, CollisionData *collisionData=NULL);
		//! Do a circle collision
		Collider *CollideWith(Collider *collider, const std::string &tag, CollisionData *collisionData=NULL);

		void SetCollider(Collider *collider);
		Collider* GetCollider();

		Collidable *Clone() const;
	protected:
		void Save(FileNode *myNode);
		void Load(FileNode *myNode);
	private:
		Collider *collider;
	};
}