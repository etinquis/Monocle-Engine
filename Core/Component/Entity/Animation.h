#pragma once

#include "../EntityComponent.h"
#include <set>
#include <string>
#include <unordered_map>

#define MONOCLE_ENTITYCOMPONENT_ANIMATION "Animation"

namespace Monocle
{
	class Anim
	{

	};

	class Animation : public EntityComponent
	{
	public:
		typedef std::unordered_map< std::string, std::list<Anim> > AnimationList;

		Animation();
		virtual ~Animation();

		virtual void Init(Entity *entity);
		virtual void Update();
		virtual void Unload();

		std::string GetName() { return MONOCLE_ENTITYCOMPONENT_ANIMATION; }

		Animation *Clone() const;
	protected:
		void Save(FileNode *myNode);
		void Load(FileNode *myNode);
	private:
		Animation(const Animation& anim);

		AnimationList animations;
	};
}