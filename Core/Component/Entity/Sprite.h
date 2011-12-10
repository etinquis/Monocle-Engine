#pragma once

#include "Graphic.h"
#include "../../Asset.h"
#include "../../Vector2.h"
#include "../../Color.h"

#include <string>

#define MONOCLE_ENTITYCOMPONENT_SPRITE "Sprite"

namespace Monocle
{
	class Entity;
	enum BlendType;
	enum FilterType;
	class TextureAsset;
	class Color;

	class Sprite : public Graphic
	{
	public:
		typedef class SpriteInitParams
		{
		private:
			std::string filename;
			FilterType filter;
			float width;
			float height;
		public:
			SpriteInitParams(std::string &filename, FilterType filter = FILTER_LINEAR, float width=-1.0, float height=-1.0)
			{
				this->filename = filename;
				this->filter = filter;
				this->width = width;
				this->height = height;
			}

		} InitParams;

		Sprite();
		~Sprite();

		std::string GetName() { return MONOCLE_ENTITYCOMPONENT_SPRITE; }
		Sprite * Clone () const { return new Sprite(*this); }

		void ParamInit(InitParams params);

		void Update();
		void Render(Entity *entity);
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
	};
}