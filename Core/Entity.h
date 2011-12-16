#pragma once

#include "Vector2.h"
#include "Scene.h"
#include "Color.h"

#include "Component/EntityComponent.h"
#include "Events/EventHandler.h"

#include <string>
#include <map>
#include <list>

namespace Monocle
{
	const float ENTITY_CONTROLPOINT_SIZE = 20.0f;

	class Scene;
	class Collision;
	class Collider;
	class RectangleCollider;
	class CircleCollider;
	class CollisionData;
	class FileNode;

	class InvokeData
	{
	public:
		InvokeData(void *me, void (*functionPointer)(void *), float delay);
		void Update();

		void (*functionPointer) (void *);
		float delay;
		bool isDone;
		void *me;
	};


	class EntityTagData
	{
	public:
		EntityTagData();
		EntityTagData(const std::string &name, bool save);
		EntityTagData(const EntityTagData &);

		std::string name;
		bool save;
	};

	typedef std::vector<EntityTagData> EntityTags;

	//! \brief An object that can Update, Render and be derived to perform other actions.
	//!
	//! Examples of what you might use entities for: players, enemies, items, menus.
	//!
	//! You would do this by deriving a new type, e.g.:
	//!
	//!		class Player : public Entity
	//! 
	//! Entities must be Added to a Scene using Scene::Add. Otherwise they will do nothing.
	//! 
	//! e.g.: in Scene::Begin()...
	//!
	//!		Add(new Player)
	//!
	class Entity
	{
	public:
		typedef std::map<std::string, EntityComponent*> ComponentList;

		class EventHandler
		{
		public:
			struct EntityEventArgs
			{
			public:
				EntityEventArgs(Entity *entity) : entity(entity) { }
				Entity *entity;
			};
			
			virtual void AddedToScene(const EntityEventArgs &args) { }
			virtual void RemovedFromScene(const EntityEventArgs &args) { }
			virtual void Destroyed(const EntityEventArgs &args) { }
			virtual void Saving(const EntityEventArgs &args) { }
			virtual void Loading(const EntityEventArgs &args) { }
		};

		EventEmitter<EventHandler> Events;

		Entity();
		virtual ~Entity();
		virtual Entity *Clone() const;

		//! Enable this object. Set isEnabled to true. Each derived Entity may decide how to handle isEnabled.
		virtual void Enable();
		//! Disable this object. Set isEnabled to false. Each derived Entity may decide how to handle isEnabled.
		virtual void Disable();
		//! \return isEnabled Is this Entity enabled?
		bool IsEnabled();

		//! Remove from Scene or Entity parent. May cause our deletion.
		void RemoveSelf();
	
		//! Called by the scene when the entity should update its game logic
		virtual void Update();

		//! Called by the scene when the entity should render
		virtual void Render();

		void Save(FileNode *fileNode);
		void Load(FileNode *fileNode);

		//! Called by the scene when the entity is added to that scene
		virtual void Added();
		//! Called by the scene when the entity is removed from that scene
		virtual void Removed();
		//! Called when Entity is destroyed
		virtual void Destroyed();

		//! Checks if this entity is on the given layer
		bool IsLayer(int layer);
		//! Gets the layer that this entity is on
		//! \return our layer number
		int GetLayer();
		//! Moves the entity to the given layer
		void SetLayer(int layer);
		//! Moves the entity to the layer determined by the given offset, relative to the layer it
		//! is currently on
		void AdjustLayer(int layerAdjustAmount);

		//! is our layer number in the debug render range?
		bool IsDebugLayer();

		//! set parent entity
		//void SetParent(Entity *parent);
		////! return pointer to parent entity
		//Entity *GetParent();
		//! return pointer to the Scene we are currently in
		Scene *GetScene();

		template <class t_component>
		t_component* AddComponent(const typename t_component::InitParams& params)
		{
			t_component *comp = new t_component();

			components[t_component::ComponentName] = comp;
			comp->ParamInit(this, params);
			return comp;
		}

		template <class t_component>
		t_component* AddComponent()
		{
			t_component *comp = new t_component();

			components[t_component::ComponentName] = comp;
			comp->Init(this);
			return comp;
		}

		bool HasComponent(const std::string &name);
		
		//Vector2 GetWorldScale(const Vector2 &scale);
		//Vector2 GetLocalPosition(const Vector2 &worldPosition);
		
		template <typename T>
		T* GetComponent()
		{
			return (T*)components[T::ComponentName];
		}
	protected:
		Entity(const Entity &entity);

		friend class Scene;

		//! The scene that contains the entity
		Scene* scene;

		bool isEnabled;

	private:
		int id;
		
		std::vector<EntityTagData> tags;
		int layer;

		std::list<InvokeData*> invokes;
		std::list<InvokeData*> removeInvokes;

        Vector2 cachedWorldPosition;
        Vector2 lastPositionWhenCached;

		ComponentList components;
	};
}
