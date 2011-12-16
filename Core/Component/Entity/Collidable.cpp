#include "Collidable.h"
#include "../../Collision.h"
#include "../../CollisionData.h"
#include "../../Debug.h"
#include "../../File/FileNode.h"

namespace Monocle
{
	const std::string Collidable::ComponentName = "Collidable";

	Collidable::Collidable()
		: collider(NULL)
	{

	}

	Collidable::Collidable(const Collidable& coll)
	{

	}

	Collidable::~Collidable() { }

	void Collidable::Unload()
	{
		Collision::RemoveCollider(collider);
		delete collider;
		collider = NULL;
	}

	Collider* Collidable::Collide(const std::string &tag, CollisionData *collisionData)
	{
		return Collision::Collide(this, tag, collisionData);
	}

	/*Collider* Collidable::CollideAt(const std::string &tag, const Vector2 &atPosition, CollisionData *collisionData)
	{
		Vector2 oldPosition = position;
		position = atPosition;
		Collider *collider = Collision::Collide(this, tag, collisionData);
		position = oldPosition;
		return collider;
	}*/

	Collider* Collidable::CollideWith(Collider *pCollider, const std::string &tag, CollisionData *collisionData)
	{
		Collider *collider = Collision::Collide(pCollider, tag, collisionData);
		return collider;
	}

	void Collidable::SetCollider(Collider *setCollider)
	{
		if (setCollider == NULL && this->collider != NULL)
		{
			// if we want to set null, and we already have a collider
			// remove the collider that we had
			Collision::RemoveCollider(this->collider);
			// set it to null
			this->collider = NULL;
			// note the code doesn't delete it
		}
		else if (this->collider != NULL)
		{
			// we could change this so that it auto-removes the existing collider instead
			Debug::Log("Error: Entity already has a collider.");
		}
		else
		{
			// set our collider pointer to the passed in collider
			this->collider = setCollider;
			// register the collider with the Collision manager
			Collision::RegisterColliderWithEntity(setCollider, this);
		}
	}

	void Collidable::Update() { }

	Collider* Collidable::GetCollider()
	{
		return collider;
	}

	Collidable *Collidable::Clone() const
	{
		return new Collidable(*this);
	}

	void Collidable::Save(FileNode *myNode)
	{
		
	}

	void Collidable::Load(FileNode *myNode)
	{

	}
}