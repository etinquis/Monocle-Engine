#include "Sprite.h"
#include "../../Assets.h"
#include "../../Entity.h"
#include "../../Graphics.h"
#include "Transform.h"

namespace Monocle
{
	Sprite::Sprite()
		: Graphic(),
		texture(NULL),
		width(1.0f),
		height(1.0f),
		textureOffset(Vector2::zero),
        textureScale(Vector2::one),
        textureOffsetModifier(Vector2::zero),
        textureScaleModifier(Vector2::one),
        trimOffset(Vector2::zero),
        trimScale(Vector2::one),
        renderOffset(Vector2::zero),
		blend(BLEND_ALPHA)
	{
	}

	void Sprite::Load(const std::string &filename, FilterType filter, float width, float height)
	{
		texture = Assets::RequestTexture(filename, filter);
		if (texture != NULL)
		{
			if (width == -1.0 || height == -1.0)
			{
				this->width = texture->width;
				this->height = texture->height;
			}
		}
	}

	void Sprite::Load(const Color& color, float width, float height)
	{
		texture = Assets::RequestColorTexture(color);
		
		if (texture != NULL)
		{
			if (width == -1.0 || height == -1.0)
			{
				this->width = texture->width;
				this->height = texture->height;
			}
		}
	}

	Sprite::~Sprite()
	{
		if (texture != NULL)
		{
			texture->RemoveReference();
			texture = NULL;
		}
	}

	void Sprite::Update()
	{
        // Prepare for premultiplied textures?
        if (blend == BLEND_ALPHA && texture->IsPremultiplied())
            blend = BLEND_ALPHA_PREMULTIPLIED;
        else if (blend == BLEND_ALPHA_PREMULTIPLIED && !texture->IsPremultiplied())
            blend = BLEND_ALPHA;
	}

	// store color info in entity?
	// that would bloat entity... hmm.
	// or make materials system...
	void Sprite::Render(Entity *entity)
	{        
		Graphics::PushMatrix();
        
        // Calculate proper offset
        Vector2 offset = trimOffset;

		Graphics::Translate( ((Transform*)(*GetEntity())["Transform"])->position.x+offset.x, ((Transform*)(*GetEntity())["Transform"])->position.y-offset.y, 0.0f);
		Graphics::Rotate( ((Transform*)(*GetEntity())["Transform"])->rotation, 0, 0, 1);
        Graphics::Scale( trimScale );
		Graphics::BindTexture(texture);
		Graphics::SetBlend(blend);
        //Graphics::SetColor(entity->color);

		/*if (Debug::showBounds && entity->IsDebugLayer())
		{
			if (Debug::selectedEntity == entity)
			{
				Color copyColor = entity->color;
				copyColor.a *= 0.8f;
				Graphics::SetColor(copyColor);
			}
			else if (Debug::selectedEntity != NULL && Debug::selectedEntity->GetLayer() > entity->GetLayer())
			{
				Color copyColor = entity->color;
				copyColor.a *= 0.6f;
				Graphics::SetColor(copyColor);
			}
		}*/

		Graphics::RenderQuad(width, height, textureOffset, textureScale);

		Graphics::PopMatrix();
		Shader::UseNone();
	}
	
    /**
        jw: why this changed.
            Previously, we were doing texture->width and texture->height.
            There are instances where the sprite drawn is NOT the size of the texture.
            >> Sprite Sheets or Animations. - The texture is bigger than the sprite used.
     **/
	void Sprite::GetWidthHeight(float *width, float *height)
	{
        *width = (float)this->width;
        *height = (float)this->height;
	}

	void Sprite::Load(FileNode *myNode)
	{

	}
	
	void Sprite::Save(FileNode *myNode)
	{

	}
}
