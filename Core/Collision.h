#pragma once

#include <string>
#include <vector>
#include <list>

#include "Vector2.h"
#include "Component/GameComponent.h"

namespace Monocle
{
	class Entity;
	class Collider;
	class CollisionData;

	//!
	//! \brief Manages collision for Monocle. In most cases there is no need to reference this class, use the Entity::Collide function instead.
	//!
	//! Collision tracks Collider instances, and uses their Entity pointers to check for which entities fit the tag description passed to Collision::Collide.
	//! 
	class Collision : public GameComponent
	{
	public:
		Collision();
		virtual ~Collision() { }

        //! Returns the first Collider with the specified tag that intersects with the line.
        //! \param start The beginning vector of the line to cast.
        //! \param end The end vector of the line to cast.
        //! \param radius The radius of the line to cast.
        //! \param tag The tag of the Colliders to be checked for intersection
        //! \param collisionData [out] The data resulting from the collision
		static Collider* LineCast(const Vector2 &start, const Vector2 &end, float radius, const std::string &tag, CollisionData *collisionData = NULL);

		std::string GetName() { return "Collision"; }

		void Init(Game* game) { GameComponent::Init(game); }
		void Update() { }
		void Unload() { }

	private:
		friend Entity;

		static void RegisterColliderWithEntity(Collider *collider, Entity *entity);
		static void RemoveCollider(Collider *collider);
		static Collider* Collide(Entity *entity, const std::string &tag, CollisionData *collisionData=NULL);
		static Collider* Collide(Collider *collider, const std::string &tag, CollisionData *collisionData=NULL);

		static Collision *instance;
		std::list<Collider*> colliders;
	};
}
