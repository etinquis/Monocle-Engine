#pragma once

#include "Graphic.h"
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
		Sprite();
		~Sprite();

		//void Load(const std::string &filename, FilterType filter = FILTER_LINEAR, float width=-1.0, float height=-1.0);
		//void Load(const Color& color, float w=-1.0, float h=-1.0);

		std::string GetName() { return MONOCLE_ENTITYCOMPONENT_SPRITE; }
		Sprite * Clone () const { return new Sprite(*this); }

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