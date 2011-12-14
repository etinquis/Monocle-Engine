#include <cstdio>
#include "Entity.h"
#include "Collision.h"
#include "Graphics.h"
#include "File/FileNode.h"
#include "MonocleToolkit.h"
#include <sstream>
#include <iostream>

#include "Component/Entity/Collidable.h"
#include "Component/Entity/Transform.h"

namespace Monocle
{
	InvokeData::InvokeData(void *me, void (*functionPointer) (void *), float delay)
		: functionPointer(functionPointer), delay(delay), isDone(false), me(me)
	{
	}

	void InvokeData::Update()
	{
		if (!isDone)
		{
			delay -= Monocle::deltaTime;
			if (delay < 0)
			{
				isDone = true;
				functionPointer(me);
			}
		}
	}

	///=====

	EntityTagData::EntityTagData()
	{
		save = false;
	}

	EntityTagData::EntityTagData(const std::string &name, bool save)
		: name(name), save(save)
	{
	}

	EntityTagData::EntityTagData(const EntityTagData &entityTagData)
		: name(entityTagData.name), save(entityTagData.save)
	{
	}

	///=====

	Entity::Entity(const Entity &entity)
		: isEnabled(true), scene(NULL), graphic(NULL), parent(NULL), layer(entity.layer)//, tags(entity.tags)
	{
        lastPositionWhenCached = Vector2(-666.6666,-666.6666);
        cachedWorldPosition = Vector2::zero;
        
		EntityTags copyTags = entity.tags;
		/*for (EntityTags::iterator i = copyTags.begin(); i != copyTags.end(); ++i)
		{
			AddTag((*i).name, (*i).save);
		}*/

		/*for (std::map<std::string, EntityComponent*>::iterator i = copyTags.begin(); i != copyTags.end(); ++i)
		{
			AddTag((*i).second.name, (*i).second.save);
		}*/
	}

	Entity::Entity()
		: isEnabled(true), scene(NULL), graphic(NULL), parent(NULL), layer(0)
		//, willDie(false)
	{
        lastPositionWhenCached = Vector2(-666.6666,-666.6666);
        cachedWorldPosition = Vector2::zero;
		
		AddComponent<Transform>();
		AddComponent<Collidable>();
	}

	Entity::~Entity()
	{
	}

	void Entity::Added()
	{
	}

	void Entity::Removed()
	{
	}

	void Entity::Destroyed()
	{
		if (graphic)
		{
			delete graphic;
			graphic = NULL;
		}

		//DestroyChildren();

		// clean up invokes
		for (std::list<InvokeData*>::iterator i = invokes.begin(); i != invokes.end(); ++i)
		{
			delete (*i);
			(*i) = NULL;
		}
		invokes.clear();
	}

	//void Entity::DestroyChildren()
	//{
	//	// clean up children
	//	for (std::list<Entity*>::iterator i = children.begin(); i != children.end(); ++i)
	//	{
	//		(*i)->Destroyed();
	//		delete (*i);
	//	}

	//	children.clear();
	//}

	void Entity::Update()
	{
		//for(std::list<Entity*>::iterator i = children.begin(); i != children.end(); ++i)
		//{
		//	(*i)->Update();
		//}

		// clean up invokes
		for(std::list<InvokeData*>::iterator i = invokes.begin(); i != invokes.end(); ++i)
		{
			(*i)->Update();

			if ((*i)->isDone)
			{
				removeInvokes.push_back(*i);
			}
		}

		/*if (graphic)
		{
			graphic->Update();
		}*/

		for (std::list<InvokeData*>::iterator i = removeInvokes.begin(); i != removeInvokes.end(); ++i)
		{
			invokes.remove(*i);
			delete (*i);
			(*i) = NULL;
		}

		for (ComponentList::iterator it = components.begin(); it != components.end(); it++)
		{
			it->second->Update();
		}
	}

	void Entity::RemoveSelf()
	{
		if (scene)
			scene->Remove(this);
	}

	void Entity::Enable()
	{
		isEnabled = true;
	}

	void Entity::Disable()
	{
		isEnabled = false;
	}

	bool Entity::IsEnabled()
	{
		return isEnabled;
	}

	void Entity::Render()
	{
        Graphics::PushMatrix();

		for(ComponentList::iterator it = components.begin(); it != components.end(); it++)
		{
			it->second->Render();
		}

		//MatrixChain();

		/*if (graphic != NULL)
		{
			Graphics::SetColor(color);
			graphic->Render(this);
		}*/
        
        Graphics::PopMatrix();
		
		//if (Debug::showBounds && IsDebugLayer())
		//{
		//	Vector2 offset;
		//	if (parent)
		//		offset = Vector2::one * 2;
  //          
		//	Graphics::BindTexture(NULL);
  //          
		//	Graphics::PushMatrix();

		//	MatrixChain();

		//	Graphics::PopMatrix();
  //          
		//	if (Debug::selectedEntity == this)
		//	{
		//		Graphics::SetColor(Color::orange);
		//	}
		//	else
		//	{
		//		Graphics::SetColor(Color(0.9f,0.9f,1.0f,0.8f));
		//	}
  //          
		//	// use world position, so we don't have to render with the matrix scale on
		//	Vector2 worldPosition = GetWorldPosition();

		//	//draw the control point
		//	Graphics::RenderLineRect(worldPosition.x, worldPosition.y, ENTITY_CONTROLPOINT_SIZE, ENTITY_CONTROLPOINT_SIZE);
  //          
		//	if (Debug::selectedEntity != this)
		//		Graphics::SetColor(Color(0.0f,0.0f,0.25f,0.8f));
  //          
		//	// draw the control point center
		//	Graphics::RenderLineRect(worldPosition.x, worldPosition.y, ENTITY_CONTROLPOINT_SIZE * 0.75f, ENTITY_CONTROLPOINT_SIZE * 0.75f);
  //          
		//	Graphics::PopMatrix();
		//}
	}

//	const std::string& Entity::GetTag(int index)
//	{
//#ifdef DEBUG
//		//Error: If the tag index to get is out of bounds
//		if (index >= tags.size())
//			Debug::Log("ERROR: Tag index out of bounds.");
//#endif
//		return tags[index].name;
//	}

//	void Entity::AddTag(const std::string& tag, bool save)
//	{
//#ifdef DEBUG
//		//Error: If the entity already has that tag
//		if (HasTag(tag))
//			Debug::Log("ERROR: Duplicate tag added to entity.");
//#endif
//		if (!HasTag(tag))
//		{
//			tags.push_back(EntityTagData(tag, save));
//			if (scene != NULL)
//				scene->EntityAddTag(this, tag);
//		}
//	}
//
//	void Entity::RemoveTag(const std::string& tag)
//	{
//#ifdef DEBUG
//		//Error: If the entity doesn't have that tag
//		if (!HasTag(tag))
//			Debug::Log("ERROR: Removing tag from an entity that doesn't have that tag.");
//#endif
//		for (EntityTags::iterator i = tags.begin(); i != tags.end(); ++i)
//		{
//			if ((*i).name.compare(tag) == 0)
//			{
//				tags.erase(i);
//				break;
//			}
//		}
//		if (scene != NULL)
//			scene->EntityRemoveTag(this, tag);
//	}
//
//	bool Entity::HasTag(const std::string& tag)
//	{
//		for (EntityTags::iterator i = tags.begin(); i != tags.end(); ++i)
//		{
//			if ((*i).name.compare(tag) == 0)
//				return true;
//		}
//
//		return false;
//	}
//
//	int Entity::GetNumberOfTags()
//	{
//		return static_cast<int>(tags.size());
//	}

	int Entity::GetLayer()
	{
		return layer;
	}

	void Entity::SetLayer(int layer)
	{
		this->layer = layer;

		//TODO: get scene to re-sort me
	}

	void Entity::AdjustLayer(int layerAdjustAmount)
	{
		this->layer += layerAdjustAmount;
	}

	bool Entity::IsLayer(int layer)
	{
		return this->layer == layer;
	}

	bool Entity::IsDebugLayer()
	{
		/*if (parent)
			return parent->IsDebugLayer();

		return layer > Debug::layerMin && layer < Debug::layerMax;*/
		return false;
	}

	/////TODO: enqueue for safety
	//// add an entity as a child
	//void Entity::Add(Entity *entity)
	//{
	//	entity->parent = this;
	//	children.push_back(entity);
	//}

	/////TODO: enqueue for safety
	//void Entity::Remove(Entity *entity)
	//{
	//	entity->parent = NULL;
	//	children.remove(entity);
	//}

	/*
	RectangleCollider* Entity::AddRectangleCollider(float width, float height, const Vector2 &offset)
	{
		return Collision::AddRectangleCollider(this, width, height, offset);
	}
	*/

	Graphic* Entity::GetGraphic()
	{
		return graphic;
	}
    
//    bool Entity::IsOnCamera( Camera *camera )
//    {
//        Graphic* graphic = GetGraphic();
//		if (graphic != NULL)
//		{
//			float biggersize, h, w;
//            
//            graphic->GetWidthHeight(&w, &h);
//            
//            // We use the greatest possible rectangle in case of rotations
//            biggersize = MAX(w,h);
//            
//			Vector2 ul = GetComponent<Transform>()->position - (Vector2(biggersize,biggersize)*0.5f* GetComponent<Transform>()->scale);
//			Vector2 lr = GetComponent<Transform>()->position + (Vector2(biggersize,biggersize)*0.5f* GetComponent<Transform>()->scale);
//            
//            float vw = Graphics::GetVirtualWidth()* camera->GetComponent<Transform>()->scale.x;
//            float vh = Graphics::GetVirtualHeight()* camera->GetComponent<Transform>()->scale.x;
//            float cx = (camera->GetComponent<Transform>()->position.x);
//            float cy = (camera->GetComponent<Transform>()->position.y);
//            
//            // As long as any one of the corners could be on screen we draw
//            return !(
//                    // What it means to be off screen:
//                     (ul.x > cx+vw) || (lr.x < cx-vw) ||
//                     (ul.y > cy+vh) || (lr.y < cy-vh)
//                    );
//            
////            printf("%.2f < %.2f && %.2f > %.2f && %.2f < %.2f && %.2f > %.2f\n",cx-vw,ul.x,cx+vw,lr.x,cy-vh,ul.y,cy+vh,lr.y);
//            
////			return (cx-vw < ul.x && cx+vw > lr.x && cy-vh < ul.y && cy+vh > lr.y);
//            
////            return (ul.x < cx+vw && ul.x > cx-vw && ul.y > cy-vh && ul.y < cy+vh) ||
////                    (lr.x > cx-vw && lr.x < cx+vw && lr.y > cy-vh && lr.y < cy+vh);
//		}
//		return true;
//    }

	/*bool Entity::IsPositionInGraphic(const Vector2 &point)
	{
		Graphic* graphic = GetGraphic();
		if (graphic != NULL)
		{
			float width, height;
			graphic->GetWidthHeight(&width, &height);
			Vector2 ul = ((Transform *)(*this)["Transform"])->GetWorldPosition(Vector2( - width * 0.5f, - height * 0.5f));
			Vector2 lr = ((Transform *)(*this)["Transform"])->GetWorldPosition(Vector2( + width * 0.5f, + height * 0.5f));
			printf("p(%d, %d) ul(%d, %d) lr(%d, %d)\n", (int)point.x, (int)point.y, (int)ul.x, (int)ul.y, (int)lr.x, (int)lr.y);
			return (point.x > ul.x && point.x < lr.x && point.y > ul.y && point.y < lr.y);
		}
		return false;
	}*/

	//void Entity::MatrixChain()
	//{
	//	std::list<Entity*> entityChain;

	//	Entity *current = this;
	//	//std::cout << typeid(*this).name() << std::endl;
	//	while (current)
	//	{
	//		entityChain.push_back(current);
	//		current = current->parent;
	//	}

	//	for (std::list<Entity*>::reverse_iterator i = entityChain.rbegin(); i != entityChain.rend(); ++i)
	//	{
	//		(*i)->ApplyMatrix();
	//		//Graphics::Translate(scene->GetCamera()->position * (*i)->followCamera + (*i)->position * (Vector2::one - (*i)->followCamera));
	//		//Graphics::Rotate((*i)->rotation, 0, 0, 1);
	//		//Graphics::Scale((*i)->scale);
	//	}
	//}

	/// TODO: write our own matrix functions to replace this stuff
	Vector2 Entity::GetLocalPosition(const Vector2 &worldPosition)
	{
		Vector2 returnPos;
		Graphics::PushMatrix();
		Graphics::IdentityMatrix();

		std::list<Entity*> entityChain;

		Entity *current = this;
		while (current)
		{
			entityChain.push_back(current);
			current = current->parent;
		}

		Graphics::Translate(worldPosition);

		for (std::list<Entity*>::iterator i = entityChain.begin(); i != entityChain.end(); ++i)
		{
			Graphics::Scale(1.0f/(*i)->GetComponent<Transform>()->scale);
			Graphics::Rotate(-(*i)->GetComponent<Transform>()->rotation, 0, 0, 1);
			Graphics::Translate(-(*i)->GetComponent<Transform>()->position);
		}

		returnPos = Graphics::GetMatrixPosition();

		Graphics::PopMatrix();

		return returnPos;
	}


	//Entity* Entity::GetChildEntityAtPosition(const Vector2 &position)
	//{
	//	for (std::list<Entity*>::iterator i = children.begin(); i != children.end(); ++i)
	//	{
	//		Entity *entity = (*i)->GetChildEntityAtPosition(position);
	//		if (entity)
	//			return entity;
	//	}
	//	
	//	if (IsPositionInGraphic(position))
	//		return this;

	//	return NULL;
	//}

	bool Entity::HasComponent(const std::string &name)
	{
		return ( components.find(name) != components.end() );
	}

	void Entity::Save(FileNode *fileNode)
	{
		//Transform::Save(fileNode);

		//if (layer != 0)
		//	fileNode->Write("layer", layer);
		/*if (color != Color::white)
			fileNode->Write("color", color);*/
		/*if (tags.size() != 0)
		{
			std::ostringstream os;
			for (EntityTags::iterator i = tags.begin(); i != tags.end(); ++i)
			{
				if ((*i).save)
					os << (*i).name << " ";
			}
			std::string saveString = os.str();
			if (!saveString.empty())
			{
				fileNode->Write("tags", os.str());
			}
		}*/
		/*if (followCamera != Vector2::zero)
			fileNode->Write("followCamera", followCamera);*/

		FileNode *node = fileNode->InsertEndChildNode("entity");

		for(ComponentList::iterator it = components.begin(); it != components.end(); it++)
		{
			it->second->SaveTo(node);
		}
	}

	void Entity::Load(FileNode *myNode)
	{
		//Transform::Load(fileNode);

		//FileNode *node = fileNode->GetChild("entity");

		for(ComponentList::iterator it = components.begin(); it != components.end(); it++)
		{
			it->second->LoadFrom(myNode);
		}

		/*fileNode->Read("color", color);*/
		/*std::string tags;
		fileNode->Read("tags", tags);
		if (tags.size() > 0)
		{
			std::string tag;
			std::istringstream is(tags);
			while (is >> tag)
			{
				AddTag(tag, true);
			}
		}*/
		/*fileNode->Read("followCamera", followCamera);*/
	}

	void Entity::SetParent(Entity *parent)
	{
		this->parent = parent;
	}

	Entity *Entity::GetParent()
	{
		return parent;
	}

	Scene* Entity::GetScene()
	{
		return scene;
	}

	///// TODO: make recursive
	//Entity *Entity::GetNearestEntityByControlPoint(const Vector2 &position, const std::string &tag, Entity *ignoreEntity, float &smallestSqrMag)
	//{
	//	Entity *nearestChild = NULL;

	//	for (std::list<Entity*>::iterator i = children.begin(); i != children.end(); ++i)
	//	{
	//		if ((*i) != ignoreEntity)
	//		{
	//			Vector2 wp = (*i)->GetWorldPosition();
	//			Vector2 diff = wp - position;
	//			//printf("wp: %f, %f\n", wp.x, wp.y);
	//			//printf("diff: %f, %f\n", diff.x, diff.y);
	//			if (diff.IsInRange(ENTITY_CONTROLPOINT_SIZE))
	//			{
	//				float sqrMag = diff.GetSquaredMagnitude();
	//				if (smallestSqrMag == -1 || sqrMag < smallestSqrMag)
	//				{
	//					smallestSqrMag = sqrMag;
	//					nearestChild = (*i);
	//				}
	//			}
	//		}

	//		Entity *newNearestChild = (*i)->GetNearestEntityByControlPoint(position, tag, ignoreEntity, smallestSqrMag);
	//		if (newNearestChild)
	//		{
	//			nearestChild = newNearestChild;
	//		}
	//	}

	//	return nearestChild;
	//}

	//const std::list<Entity*>* Entity::GetChildren()
	//{
	//	return &children;
	//}

	
	/*void Entity::Invoke(void (*functionPointer)(void*), float delay)
	{
		invokes.push_back(new InvokeData((void*)this, functionPointer, delay));
	}*/

	/*
	void Entity::Die()
	{
		if (scene)
		{
			willDie = true;
			scene->Remove(this);
		}
		else
		{
			Debug::Log("Error: Entity is not in a scene. 'Die' will not function.");
		}
	}
	*/

	/*
	void Entity::CleanUp()
	{
		std::list<Entity*> toKill;
		for (std::list<Entity*>::iterator i = children.begin(); i != children.end(); ++i)
		{
			if ((*i)->willDie)
			{
				toKill.push_back(*i);
			}
			(*i)->CleanUp();
		}

		for (std::list<Entity*>::iterator i = toKill.begin(); i != toKill.end(); ++i)
		{
			children.remove(*i);
		}
	}
	*/

	Entity *Entity::Clone() const
	{
		return new Entity(*this);
	}
}
