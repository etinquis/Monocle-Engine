#include "Puppet.h"

#include <TinyXML/tinyxml.h>
#include <Assets.h>
#include <Graphics/Sprite.h>
#include <Monocle.h>
#include <TextureAtlas.h>

#include <sstream>

namespace Monocle
{
	
	/// PART
	Part::Part(int id, const std::string &name, const std::string &imageFilename)
		: Entity(), id(id), name(name), sprite(NULL), puppet(NULL)
	{
		sprite = new Sprite(imageFilename);
		SetGraphic(sprite);
	}

	Part::Part()
		: Entity(), id(-1), sprite(NULL), puppet(NULL)
	{

	}

	int Part::GetID()
	{
		return id;
	}

	bool Part::IsName(const std::string &name)
	{
		return this->name == name;
	}

	std::string Part::GetName()
	{
		return this->name;
	}

	bool Part::IsID(int id)
	{
		return this->id == id;
	}

	Sprite *Part::GetSprite()
	{
		return sprite;
	}

	void Part::SetPuppet(Puppet *puppet)
	{
		this->puppet = puppet;
	}

	void Part::SaveTo(FileNode *parentNode)
	{
		FileNode *myNode = parentNode->InsertEndChildNode("Part");
		//Entity::Save(fileNode);
		myNode->Write("id", id);
		myNode->Write("name", name);
		
		myNode->Write("atlasEntry", atlasEntry);

		if (sprite)
		{
			if (atlasEntry == "")
			{
				myNode->Write("image", sprite->texture->GetName());
				myNode->Write("width", sprite->width);
				myNode->Write("height", sprite->height);
			}
			
			if (sprite->position != Vector2::zero)
				myNode->Write("offset", sprite->position);
		}

		int layer = GetLayer();
		if (layer != 0)
			myNode->Write("layer", layer);

		//std::ostringstream os;
		//os << sprite->textureOffset.x << " " << sprite->textureOffset.y << " " << sprite->textureScale.x << " " << sprite->textureScale.y;
		//os.str();
	}

	void Part::LoadFrom(FileNode *myNode)
	{
		//Entity::Load(fileNode);

		myNode->Read("id", id);
		myNode->Read("name", name);

		sprite = NULL;
		SetGraphic(NULL);

		TextureAtlas *textureAtlas = puppet->GetTextureAtlas();
		if (textureAtlas)
		{
			TextureAtlasEntry *textureAtlasEntry = NULL;
			myNode->Read("atlasEntry", atlasEntry);
			if (atlasEntry != "")
			{
				textureAtlasEntry = textureAtlas->GetEntryByName(atlasEntry);
				if (textureAtlasEntry)
				{
					sprite = new Sprite(textureAtlas->GetImageName());
					SetGraphic(sprite);
					sprite->textureOffset = textureAtlasEntry->GetTextureOffset();
					sprite->textureScale = textureAtlasEntry->GetTextureScale();
					printf("textureOffset: (%f, %f) textureScale: (%f, %f)\n", sprite->textureOffset.x, sprite->textureOffset.y, sprite->textureScale.x, sprite->textureScale.y);
					sprite->width = textureAtlasEntry->GetWidth();
					sprite->height = textureAtlasEntry->GetHeight();
				}
			}
		}

		if (sprite == NULL)
		{
			std::string image;
			myNode->Read("image", image);

			sprite = new Sprite(image);
			SetGraphic(sprite);
		}
		
		if (sprite != NULL)
		{
			if (atlasEntry == "")
			{
				int width=-1, height=-1;
				myNode->Read("width", width);
				myNode->Read("height", height);

				if (width != -1)
				{
					sprite->width = width;
				}

				if (height != -1)
				{
					sprite->height = height;
				}
			}

			myNode->Read("offset", sprite->position);
		}

		int layer = 0;
		myNode->Read("layer", layer);
		SetLayer(layer);

//		if (sprite)
//		{
//			std::string atlas;
//			fileNode->Read("atlas", atlas);
//			if (atlas != "")
//			{
//				int x, y, width, height, maxWidth=-1, maxHeight=-1;
//				std::istringstream is(atlas);
//				is >> x >> y >> width >> height >> maxWidth >> maxHeight;
//
//				if (maxWidth == -1)
//				maxWidth = width;
//				if (maxHeight == -1)
//				maxHeight = height;
//
//				sprite->textureOffset = Vector2(x/float(maxWidth), y/float(maxHeight));
//				sprite->textureScale = Vector2(width/float(maxWidth), height/float(maxHeight));
//
//				sprite->width = width;
//				sprite->height = height;
//			}
//		}
	}

    KeyFrame::KeyFrame(float time, const Entity &entity)
        : Transform(entity), time(time)
    {
    }
    
	KeyFrame::KeyFrame()
		: Transform(), easeType(EASE_LINEAR)
	{
	}


   	float KeyFrame::GetTime() const
	{
		return time;
	}

	void KeyFrame::SetTime(float time)
	{
		this->time = time;
	}

	void KeyFrame::SaveTo(FileNode *parentNode)
	{
		FileNode *myNode = parentNode->InsertEndChildNode("KeyFrame");
		myNode->Write("position", position);
		myNode->Write("scale", scale);
		myNode->Write("rotation", rotation);
		myNode->Write("time", time);
	}

	void KeyFrame::LoadFrom(FileNode *myNode)
	{
		Transform::LoadFrom(myNode);

		myNode->Read("time", time);
	}

    
	PartKeyFrames::PartKeyFrames()
		: part(NULL), puppet(NULL)
	{
	}
    
    PartKeyFrames::PartKeyFrames(Part *part)
        : part(part), puppet(NULL)
    {
    }

	std::list<KeyFrame> *PartKeyFrames::GetKeyFrames()
	{
		return &keyFrames;
	}

	void PartKeyFrames::SetPuppet(Puppet *puppet)
	{
		this->puppet = puppet;
	}

	void PartKeyFrames::GetKeyFrameForTime(float time, KeyFrame **prev, KeyFrame **next)
	{
		// go through all the keyframes, check time
		KeyFrame *lastKeyFrame = NULL;
		for (std::list<KeyFrame>::iterator i = keyFrames.begin(); i != keyFrames.end(); ++i)
		{
			KeyFrame *keyFrame = &(*i);
			if (time > keyFrame->GetTime())
			{
				*prev = keyFrame;
			}
			else
			{
				*next = keyFrame;
				return;
			}
			lastKeyFrame = keyFrame;
		}
	}

	void PartKeyFrames::SaveTo(FileNode *parentNode)
	{
		FileNode *myNode = parentNode->InsertEndChildNode("PartKeyFrames");
		myNode->Write("id", part->GetID());

		for(std::list<KeyFrame>::iterator it = keyFrames.begin(); it != keyFrames.end(); it++)
		{
			it->SaveTo(myNode);
		}
	}

	void PartKeyFrames::LoadFrom(FileNode *myNode)
	{
		int id;
		myNode->Read("id", id, "-1");
		SetPart(puppet->GetPartByID(id));

		std::list<FileNode *> kflist = myNode->GetChildren("KeyFrame");
		for(std::list<FileNode *>::iterator it = kflist.begin(); it != kflist.end(); it++)
		{
			KeyFrame kf;
			kf.LoadFrom(*it);
			keyFrames.push_back(kf);
		}
	}

	void PartKeyFrames::SetPart(Part *part)
	{
		this->part = part;
	}

	inline Part* PartKeyFrames::GetPart()
	{
		return part;
	}

	void PartKeyFrames::AddKeyFrame(const KeyFrame &keyFrame)
	{
		keyFrames.push_back(keyFrame);
	}

	KeyFrame *PartKeyFrames::GetLastKeyFrame()
	{
		if (keyFrames.size() == 0)
			return NULL;
		return &keyFrames.back();
	}
    
    void PartKeyFrames::InsertKeyFrame(const KeyFrame &keyFrame)
    {
		for (std::list<KeyFrame>::iterator i = keyFrames.begin(); i != keyFrames.end(); ++i)
		{
			if (keyFrame.GetTime() == (*i).GetTime())
			{
				(*i) = keyFrame;
				return;
			}
			else if ((*i).GetTime() > keyFrame.GetTime())
			{
				//inserts BEFORE iterator
				keyFrames.insert(i, keyFrame);
				return;
			}
		}
		keyFrames.push_back(keyFrame);
    }


	Animation::Animation()
		: currentTime(0.0f), duration(0.0f)
	{
	}

	void Animation::Update()
	{
		currentTime += Monocle::deltaTime;

        ApplyTimeChange();
	}
    
    void Animation::ApplyTimeChange(bool loop)
    {
		if (currentTime >= duration)
		{
			if (!loop)
				currentTime = duration;
			else
				currentTime -= int(currentTime/duration) * duration;
		} 
		else if (currentTime < 0)
		{
			if (!loop)
				currentTime = 0;
			else
				currentTime += (int(-1*currentTime/duration)+1) * duration;
		}

		for (std::list<PartKeyFrames>::iterator i = partKeyFrames.begin(); i != partKeyFrames.end(); ++i)
		{
			PartKeyFrames *currentPartKeyFrames = &(*i);
			if (currentPartKeyFrames)
			{
				KeyFrame *prev=NULL, *next=NULL;
				currentPartKeyFrames->GetKeyFrameForTime(currentTime, &prev, &next);
				if (prev && !next)
				{
					currentPartKeyFrames->GetPart()->LerpTransform(prev, prev, 1.0f);
				}
				else if (prev && next)
				{
					float diff = next->GetTime() - prev->GetTime();
					float p = (currentTime - prev->GetTime()) / diff;
                    
					//printf("LerpTransform %f\n", p);
					// adjust p by ease
					currentPartKeyFrames->GetPart()->LerpTransform(prev, next, Tween::Ease(p, EASE_INOUTSIN));//prev->easeType));
				}
			}
		}
    }

	std::string Animation::GetName()
	{
		return name;
	}
    
    float Animation::GetDuration()
    {
        return duration;
    }

    float Animation::GetCurrentTime()
    {
        return currentTime;
    }
    
    PartKeyFrames *Animation::GetPartKeyFrames(Part *part)
    {
        for(std::list<PartKeyFrames>::iterator i = partKeyFrames.begin(); i != partKeyFrames.end(); ++i)
        {
            if ((*i).GetPart() == part)
            {
                return &(*i);
            }
        }
        return NULL;
    }
    
    void Animation::SetPartKeyFrame(Part *part, const KeyFrame &keyFrame)
    {
        PartKeyFrames *partKeyFrames = GetPartKeyFrames(part);
        if (!partKeyFrames)
        {
            AddPartKeyFrames(PartKeyFrames(part));
            partKeyFrames = GetPartKeyFrames(part);
        }
        if (partKeyFrames)
        {
            partKeyFrames->InsertKeyFrame(keyFrame);
        }
    }

	bool Animation::IsName(const std::string &name)
	{
		return (this->name == name);
	}

	void Animation::AddPartKeyFrames(const PartKeyFrames &partKeyFrames)
	{
		this->partKeyFrames.push_back(partKeyFrames);
	}
	
	void Animation::SetDuration(float duration)
	{
		this->duration = duration;
	}

	void Animation::SetCurrentTime(float currentTime)
	{
		this->currentTime = currentTime;
		ApplyTimeChange();
	}
    
    void Animation::AdjustCurrentTime(float timeOffset, bool loop)
    {
        currentTime += timeOffset;
        ApplyTimeChange(loop);
    }

	void Animation::RefreshDuration()
	{
		duration = -1.0f;
		for (std::list<PartKeyFrames>::iterator i = partKeyFrames.begin(); i != partKeyFrames.end(); ++i)
		{
			KeyFrame *keyFrame = (*i).GetLastKeyFrame();
			if (keyFrame)
			{
				float t = keyFrame->GetTime();
				if (t > duration)
					duration = t;
			}
		}
	}

	void Animation::SaveTo(FileNode *parentNode)
	{
		FileNode *myNode = parentNode->InsertEndChildNode("Animation");
		myNode->Write("name", name);

		for(std::list<PartKeyFrames>::iterator it = partKeyFrames.begin(); it != partKeyFrames.end(); it++)
		{
			it->SaveTo(myNode);
		}
	}

	void Animation::LoadFrom(FileNode *myNode)
	{
		myNode->Read("name", name);

		std::list<FileNode*> pkflist = myNode->GetChildren("PartKeyFrames");
		for(std::list<FileNode*>::iterator it = pkflist.begin(); it != pkflist.end(); it++)
		{
			PartKeyFrames pkf;
			pkf.LoadFrom(*it);
			partKeyFrames.push_back(pkf);
		}
	}

	Puppet::Puppet()
		: isPlaying(false), isPaused(false), textureAtlas(NULL), currentAnimation(NULL)
	{
	}
	
	Puppet::~Puppet()
	{
		if (textureAtlas)
		{
			delete textureAtlas;
			textureAtlas = NULL;
		}
	}

	void Puppet::SaveTo(FileNode *parentNode)
	{
		FileNode *puppet = parentNode->InsertEndChildNode("Puppet");
		textureAtlas->SaveTo(puppet);
		FileNode *partlist = parentNode->InsertEndChildNode("Parts");
		
		for (std::list<Part*>::iterator i = parts.begin(); i != parts.end(); ++i)
		{
			if ((*i)->GetParent() == entity)
			{
				printf("calling SaveParts... top level...\n");
				(*i)->SaveTo(partlist);
			}
		}

		FileNode *animationlist = parentNode->InsertEndChildNode("Animations");

		for (std::list<Animation>::iterator i = animations.begin(); i != animations.end(); ++i)
		{
			Animation *animation = &(*i);
            i->SaveTo(animationlist);
		}
	}

	void Puppet::LoadFrom(FileNode *myNode)
	{
		animations.clear();
		parts.clear();

		FileNode *texAtlas = myNode->GetChild("TextureAtlas");
		textureAtlas->LoadFrom(texAtlas);

		std::list<FileNode *>partlist = myNode->GetChildren("Parts");
		for(std::list<FileNode *>::iterator it = partlist.begin(); it != partlist.end(); it++)
		{
			Part *p = new Part();
			p->LoadFrom((*it));
			parts.push_back(p);
		}

		std::list<FileNode *>animationlist = myNode->GetChildren("Animations");
		for(std::list<FileNode *>::iterator it = animationlist.begin(); it != animationlist.end(); it++)
		{
			Animation a;
			a.LoadFrom((*it));
			animations.push_back(a);
		}
	}

	void Puppet::Play(const std::string &animationName, bool isLooping)
	{
		currentAnimation = GetAnimationByName(animationName);

		if (currentAnimation)
		{
			this->isPlaying = true;
			this->isLooping = isLooping;
		}

	}

	void Puppet::Update()
	{
		if (currentAnimation)
		{
			if (isPlaying && !isPaused)
			{
				currentAnimation->Update();
			}
		}
	}

	void Puppet::Stop()
	{
		isPlaying = false;
		isPaused = false;
	}

	void Puppet::Pause()
	{
		isPaused = true;
	}

	void Puppet::Resume()
	{
		isPaused = false;
	}

	void Puppet::TogglePause()
	{
		isPaused = !isPaused;
	}

	bool Puppet::IsPlaying()
	{
		return isPlaying;
	}

	bool Puppet::IsPaused()
	{
		return isPaused;
	}
    
    Animation *Puppet::GetCurrentAnimation()
    {
        return currentAnimation;
    }
	
	TextureAtlas *Puppet::GetTextureAtlas()
	{
		return textureAtlas;
	}

	Animation *Puppet::GetAnimationByName(const std::string &name)
	{
		for (std::list<Animation>::iterator i = animations.begin(); i != animations.end(); ++i)
		{
			return &(*i);
		}
		return NULL;
	}

	Part *Puppet::GetPartByName(const std::string &name)
	{
		for (std::list<Part*>::iterator i = parts.begin(); i != parts.end(); ++i)
		{
			if ((*i)->IsName(name))
			{
				return *i;
			}
		}
		return NULL;
	}

	Part *Puppet::GetPartByID(int id)
	{
		for (std::list<Part*>::iterator i = parts.begin(); i != parts.end(); ++i)
		{
			if ((*i)->IsID(id))
			{
				return *i;
			}
		}
		return NULL;
	}
}
