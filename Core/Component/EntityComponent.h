#pragma once

#include "Component.h"

namespace Monocle
{
	class Entity;
	class FileNode;

	class EntityComponent : public Component
	{
	public:
		EntityComponent();
		virtual ~EntityComponent();

		virtual void Init(Entity* entity);

		virtual void Render();

		void SaveTo(FileNode *parentNode);
		void LoadFrom(FileNode *parentNode);

		Entity* GetEntity();
	protected:
		Entity* entity;

		EntityComponent(const EntityComponent &ecp);

		virtual void Save(FileNode *myNode) = 0;
		virtual void Load(FileNode *myNode) = 0;
	};
}