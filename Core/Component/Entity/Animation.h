#pragma once

#include "../EntityComponent.h"
#include <set>
#include <string>
#include <map>
#include <list>

namespace Monocle
{
	class Anim
	{
		public:
		Anim(const std::string &name, int start, int end, float speed, bool loop=true);
        //Anim(const std::string &name, ZwopSprite *firstSprite, int numFrames, float speed, bool loop=true);
		Anim();
		bool isPlaying;
		const std::string name;
		float frame;
		float speed;
		int start;
		int end;
        bool loop;
        
        //ZwopSprite *firstSprite;
	};

	class Animation : public EntityComponent
	{
	public:
		typedef std::map< std::string, std::list<Anim> > AnimationList;
		typedef class AnimationInitParams
		{

		} InitParams;

		static const std::string ComponentName;

		Animation();
		virtual ~Animation();

		virtual void Init(Entity *entity);
		virtual void Update();
		virtual void Unload();

		std::string GetName() { return Animation::ComponentName; }

		Animation *Clone() const;
	protected:
		void Save(FileNode *myNode);
		void Load(FileNode *myNode);
	private:
		Animation(const Animation& anim);

		AnimationList animations;
	};
}