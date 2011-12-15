#include "Sprite.h"
#include "../../Assets.h"
#include "../../Entity.h"
#include "../../Graphics.h"
#include "../../TextureAsset.h"
#include "Transform.h"
#include "../../File/FileNode.h"

namespace Monocle
{
	const std::string Sprite::ComponentName = MONOCLE_ENTITYCOMPONENT_SPRITE;

	Sprite::Sprite()
		: texture(NULL),
		width(-1.0f),
		height(-1.0f),
		textureOffset(Vector2::zero),
        textureScale(Vector2::one),
        textureOffsetModifier(Vector2::zero),
        textureScaleModifier(Vector2::one),
        trimOffset(Vector2::zero),
        trimScale(Vector2::one),
        renderOffset(Vector2::zero),
		blend(BLEND_ALPHA)
	{
		AddDependency<Transform>();
	}

	void Sprite::LoadGraphic(const std::string &filename, FilterType filter, float width, float height)
	{
		texture = Assets::RequestTexture(filename, filter);
		if (texture != NULL)
		{
			
			if (width == -1.0 || height == -1.0)
			{
				this->width = texture->width;
				this->height = texture->height;
			}
			else
			{
				if(this->width == -1.0 || this->height == -1.0)
				{
					this->width = texture->width;
					this->height = texture->height;
				}
			}
		}
	}

	Sprite::~Sprite()
	{
		
	}

	void Sprite::Unload()
	{
		EntityComponent::Unload();
		if (texture != NULL)
		{
			texture->RemoveReference();
			texture = NULL;
		}
	}

	void Sprite::Init(Entity *entity)
	{
		EntityComponent::Init(entity);
		transform = entity->GetComponent<Transform>();
	}

	void Sprite::ParamInit(Entity *entity, const Sprite::InitParams& params)
	{
		Init(entity);
		LoadGraphic(params.filename, params.filter, params.width, params.height);
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
	void Sprite::Render()
	{        
		Graphics::PushMatrix();
        
        // Calculate proper offset
        Vector2 offset = trimOffset;

		Graphics::Translate( transform->position.x+offset.x, transform->position.y-offset.y, 0.0f);
		Graphics::Rotate( transform->rotation, 0, 0, 1);
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
		Color color = Color::white;
		//FilterType filter = FILTER_LINEAR;
		std::string filename;
		//myNode->Read("filtertype", filter);
		myNode->Read("filename", filename);
		myNode->Read("color", color);
		myNode->Read("tint", color);
		myNode->Read("offset", textureOffset);
		myNode->Read("scale", textureScale);
		myNode->Read("width", width);
		myNode->Read("height", height);
	}
	
	void Sprite::Save(FileNode *myNode)
	{
		myNode->Write("tint", color);
		myNode->Write("offset", textureOffset);
		myNode->Write("scale", textureScale);
		myNode->Write("width", width);
		myNode->Write("height", height);
	}
}
