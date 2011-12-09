#pragma once

#include <list>
#include "../../Entity.h"
//#include "../Tween.h"
//#include "Component/Entity/Sprite.h"

#define MONOCLE_ENTITYCOMPONENT_PUPPET "Puppet"

namespace Monocle
{
	class Puppet;
	enum EaseType;
    
	class Part : public Entity
	{
	public:
		Part(int id, const std::string &name, const std::string &imageFilename);
		Part();

		Part *Clone() const;

		int GetID();
		std::string GetName();
		
	private:
		friend class Puppet;
		int id;
		std::string atlasEntry;
		std::string name;
		Puppet *puppet;

		std::list<Part*> parts;

		virtual void Load(FileNode *node);
		virtual void Save(FileNode *node);
	};

	class KeyFrame : public Entity
	{
	public:
        KeyFrame(float time, const Entity &entity);
		KeyFrame();
        float GetTime() const;
		void SetTime(float time);

		void Save(FileNode *fileNode);
		void Load(FileNode *fileNode);

		EaseType easeType;

	private:
		float time;
	};

	class PartKeyFrames
	{
	public:
        PartKeyFrames(Part *part);
		PartKeyFrames();
		void AddKeyFrame(const KeyFrame &keyFrame);
		void SetPart(Part *part);
		Part *GetPart();
		void GetKeyFrameForTime(float time, KeyFrame **prev, KeyFrame **next);
		KeyFrame *GetLastKeyFrame();
		std::list<KeyFrame> *GetKeyFrames();
        void InsertKeyFrame(const KeyFrame &keyFrame);

		void SetPuppet(Puppet *puppet);

		void Save(FileNode *fileNode);
		void Load(FileNode *fileNode);
		
	private:
		std::list<KeyFrame> keyFrames;
		Part *part;
		Puppet *puppet;
		//int id;
	};

	/*class Animation
	{
	public:
		Animation();
		void Update();
        void ApplyTimeChange(bool loop=true);
		void SetCurrentTime(float time);
        void AdjustCurrentTime(float timeOffset, bool loop=true);
        
		std::string GetName();
		bool IsName(const std::string &name);
		void AddPartKeyFrames(const PartKeyFrames &partKeyFrames);
        void SetPartKeyFrame(Part *part, const KeyFrame &keyFrame);
		void SetDuration(float length);
        float GetDuration();
        float GetCurrentTime();
        PartKeyFrames *GetPartKeyFrames(Part *part);
        
		void RefreshDuration();

		void Save(FileNode *fileNode);
		void Load(FileNode *fileNode);
        
	private:
		friend class Puppet;
		float currentTime;
		float duration;
		std::string name;
		std::list<PartKeyFrames> partKeyFrames;
	};*/

	class TextureAtlas;
	
	class Puppet : public EntityComponent
	{
	public:
		Puppet();
		~Puppet();

		std::string GetName() { return MONOCLE_ENTITYCOMPONENT_PUPPET; }
		Puppet *Clone() const { return new Puppet(*this); }

		void Save();
		void Load(const std::string &filename, Entity *entity);
		//void Load(FileNode *node);

		void Play(const std::string &animationName, bool isLooping=true);
		void Stop();
		void Pause();
		void Resume();

		void TogglePause();

		void Added();
		void Update();

		bool IsPlaying();
		bool IsPaused();

		Part *GetPartByName(const std::string &name);
		Part *GetPartByID(int id);

		TextureAtlas *GetTextureAtlas();

		void AdjustTime(float time);
		
	protected:
		virtual void Save(FileNode *myNode);
		virtual void Load(FileNode *myNode);

	private:
		Puppet(const Puppet &p);

		//Animation *GetAnimationByName(const std::string &animName);
		bool isPlaying;
		bool isPaused;
		bool isLooping;
	
		//Animation *currentAnimation;
	
		//std::list<Animation> animations;
		std::list<Part*> parts;
		
		TextureAtlas *textureAtlas;

		std::string filename;

		void SavePart(TiXmlElement *parentElement, Part *part);
		void LoadParts(TiXmlElement *element, Part *intoPart);
	};

}
