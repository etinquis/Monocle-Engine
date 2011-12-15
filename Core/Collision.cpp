#include "Collision.h"

#include <stdio.h>

#include "Entity.h"
#include "Colliders/Collider.h"
#include "Component/Entity/Collidable.h"
#include "CollisionData.h"

namespace Monocle
{
	Collision* Collision::instance = NULL;

	const std::string Collision::ComponentName = "Collision";

	Collision::Collision() : GameComponent()
	{
		instance = this;
	}

	void Collision::RegisterColliderWithEntity(Collider *collider, Collidable *coll)
	{
		collider->SetCollidable(coll);
		instance->colliders.push_back(collider);
		// old: //entity->SetCollider(collider);
	}

	void Collision::RemoveCollider(Collider *collider)
	{
		instance->colliders.remove(collider);
	}

	Collider* Collision::LineCast(const Vector2 &start, const Vector2 &end, float radius, const std::string &tag, CollisionData *collisionData)
	{
		Collider *collider = NULL;
		for (std::list<Collider*>::iterator i = instance->colliders.begin(); i != instance->colliders.end(); ++i)
		{
			if ((*i)->GetCollidable()->HasTag(tag))
			{
				if ((*i)->IntersectsLine(start, end, radius, collisionData))
				{
					return collisionData->collider;
				}
			}
		}
		return collider;
	}

	Collider* Collision::Collide(Collidable *entity, const std::string &tag, CollisionData *collisionData)
	{
		Collider *passedCollider = entity->GetCollider();
		if (passedCollider != NULL)
		{
			for (std::list<Collider*>::iterator i = instance->colliders.begin(); i != instance->colliders.end(); ++i)
			{
				Collider* otherEntity = *i;
				if (otherEntity != NULL && otherEntity->GetCollidable()->GetEntity() != entity->GetEntity())
				{
					Collider* otherCollider = (*i);
					if (otherCollider != NULL && otherEntity->GetCollidable()->HasTag(tag))
					{
						if (Collider::Collide(passedCollider, otherCollider, collisionData))
						{
							return otherCollider;
						}
					}
				}
			}
		}
		return NULL;
	}

	Collider* Collision::Collide(Collider *passedCollider, const std::string &tag, CollisionData *collisionData)
	{
		if (passedCollider != NULL)
		{
			for (std::list<Collider*>::iterator i = instance->colliders.begin(); i != instance->colliders.end(); ++i)
			{
				Collidable* otherEntity = (*i)->GetCollidable();
				if (otherEntity != NULL) //&& otherEntity != entity)
				{
					Collider* otherCollider = otherEntity->GetCollider();
					if (otherCollider != NULL && otherEntity->HasTag(tag))
					{
						if (Collider::Collide(passedCollider, otherCollider, collisionData))
						{
							return otherCollider;
						}
					}
				}
			}
		}
		return NULL;
	}

	//OPTION: refactor to add multiple colliders? (consider carefully)
	//FORNOW: limit to one collider per entity (simpler, makes more sense)
	//LATER: rename these to "Set" or something else?

	/*
	CircleCollider* Collision::AddCircleCollider(Entity *entity, float radius, Vector2 offset)
	{
		if (entity->GetCollider() == NULL)
		{
			CircleCollider* circleCollider = new CircleCollider(radius, offset);

			RegisterColliderWithEntity((Collider*)circleCollider, entity);

			return circleCollider;
		}

		Debug::Log("ERROR: Cannot add more than one Collider to an Entity.");
		return NULL;
	}

	RectangleCollider* Collision::AddRectangleCollider(Entity *entity, float width, float height, Vector2 offset)
	{
		if (entity->GetCollider() == NULL)
		{
			RectangleCollider *rectangleCollider = new RectangleCollider(width, height, offset);
			 
			RegisterColliderWithEntity((Collider*)rectangleCollider, entity);

			return rectangleCollider;
		}

		Debug::Log("ERROR: Cannot add more than one Collider to an Entity.");
		return NULL;
	}
	*/

	Collision *Collision::Clone() const
	{
		return new Collision(*this);
	}
}
