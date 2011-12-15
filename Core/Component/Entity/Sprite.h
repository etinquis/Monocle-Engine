#pragma once

#include "Graphic.h"
#include "../../Asset.h"
#include "../../Vector2.h"
#include "../../Color.h"

#include "Graphics.h"

#include <string>

#define MONOCLE_ENTITYCOMPONENT_SPRITE "Sprite"

namespace Monocle
{
	class Entity;
	class Transform;
	enum BlendType;
	enum FilterType;
	class TextureAsset;
	class Color;

	class Sprite : public EntityComponent
	{
	public:
		typedef class SpriteInitParams
		{
		public:
			std::string filename;
			FilterType filter;
			float width;
			float height;

			SpriteInitParams(const std::string &filename, FilterType filter = FILTER_LINEAR, float width=-1.0, float height=-1.0)
			{
				this->filename = filename;
				this->filter = filter;
				this->width = width;
				this->height = height;
			}

		} InitParams;

		Sprite();
		~Sprite();
		
		static const std::string ComponentName;

		const std::string& GetName() { return Sprite::ComponentName; }
		Sprite *Clone () const { return new Sprite(*this); }

		void Init(Entity *entity);
		void Unload();
		void ParamInit(Entity *entity, const InitParams& params);

		void Update();
		void Render();
		void GetWidthHeight(float *width, float *height);

		TextureAsset *texture;
		float width, height;//,angle;
		Vector2 textureOffset;
		Vector2 textureScale;
        
        // Original textureOffset and textureScale (for animations within spritesheets)
		Vector2 textureOffsetModifier;
		Vector2 textureScaleModifier;
        
        // Render offset (use for hotspots in animations, etc)
        // This essentially is how far from position we'll render (but we want to retain our position for worldposition purposes)
        Vector2 renderOffset;
        
        // Offsets (for trimmed sprites)
        Vector2 trimOffset;
        Vector2 trimScale;

		BlendType blend;

		Color color;

	protected:
		virtual void Save(FileNode *myNode);
		virtual void Load(FileNode *myNode);

		virtual void LoadGraphic(const std::string &filename, FilterType filter, float width, float height);
	private:
		Transform *transform;
	};

}