#include "FringeTile.h"
#include "../Assets.h"
#include "../Level/Level.h"
#include "Component/Entity/Transform.h"
#include "Component/Entity/Sprite.h"
#include "../File/FileNode.h"

#include <cstdio>

namespace Monocle
{
	FringeTileset::FringeTileset(const std::string &name)
		: name(name), maxTileID(0)
	{
	}

	FringeTileset::~FringeTileset()
	{
		// delete
	}

	bool FringeTileset::IsName(const std::string &name)
	{
		return this->name == name;
	}

	const std::string &FringeTileset::GetName()
	{
		return this->name;
	}

	const FringeTileData *FringeTileset::GetFringeTileDataByID(int tileID)
	{
		return fringeTileData[tileID];
	}

	void FringeTileset::SetFringeTileData(int tileID, FringeTileData *fringeTileData)
	{
		this->fringeTileData[tileID] = fringeTileData;
		if ((tileID+1) > maxTileID) maxTileID = tileID+1;
	}
	
	int FringeTileset::GetNextTileID(int tileID)
	{
		if (maxTileID < 1) return -1;

		do
		{
			tileID++;
			if (tileID >= maxTileID)
				tileID = 0;
		}
		while (!fringeTileData[tileID]);
		return tileID;
	}

	int FringeTileset::GetPrevTileID(int tileID)
	{
		if (maxTileID < 1) return -1;

		do
		{
			tileID--;
			if (tileID < 0)
				tileID = maxTileID-1;
		}
		while (!fringeTileData[tileID]);

		return tileID;
	}

	FringeTile::FringeTile()
		: Entity(), tileID(0)
	{
		AddComponent<Sprite>();
	}

	FringeTile::FringeTile(const FringeTile &fringeTile)
		: Entity(fringeTile), tileID(fringeTile.tileID)
	{
		
	}

	FringeTile* FringeTile::Clone()
	{
		return new FringeTile(*this);
	}
	
	void FringeTile::SetTileID(int tileID)
	{
		this->tileID = tileID;
		RefreshTexture();
	}

	int FringeTile::GetTileID()
	{
		return tileID;
	}

	void FringeTile::NextTile()
	{
		tileID = Level::GetCurrentFringeTileset()->GetNextTileID(tileID);
		RefreshTexture();
	}

	void FringeTile::PrevTile()
	{
		tileID = Level::GetCurrentFringeTileset()->GetPrevTileID(tileID);
		RefreshTexture();
	}

	/*
	FringeTileset *FringeTile::GetFringeTileset()
	{
		return fringeTileset;
	}
	*/

	void FringeTile::NextBlend()
	{
		int spriteBlend = GetComponent<Sprite>()->blend;
		spriteBlend++;
		GetComponent<Sprite>()->blend = (BlendType)spriteBlend;
	}

	void FringeTile::PrevBlend()
	{
		int spriteBlend = GetComponent<Sprite>()->blend;
		spriteBlend--;
		GetComponent<Sprite>()->blend = (BlendType)spriteBlend;
	}

	void FringeTile::Update()
	{
		Entity::Update();
		
		RefreshScale();
	}

	void FringeTile::RefreshScale()
	{
		if (GetComponent<Sprite>()->texture && (GetComponent<Sprite>()->texture->repeatX || GetComponent<Sprite>()->texture->repeatY))
		{
			const FringeTileData *fringeTileData = Level::GetCurrentFringeTileset()->GetFringeTileDataByID(tileID);
			if (fringeTileData->autoTile)
			{
				if (GetComponent<Sprite>()->texture->repeatX)
				{
					GetComponent<Sprite>()->textureScale.x = GetComponent<Transform>()->scale.x;
				}
				if (GetComponent<Sprite>()->texture->repeatY)
				{
					GetComponent<Sprite>()->textureScale.y = GetComponent<Transform>()->scale.y;
				}
			}

		}
	}

	void FringeTile::RefreshTexture()
	{
		//printf("RefreshTexture to tileID: %d\n", tileID);

		// free old texture here somehow:
		if (GetComponent<Sprite>()->texture)
		{
			GetComponent<Sprite>()->texture->RemoveReference();
			GetComponent<Sprite>()->texture = NULL;
		}

		const FringeTileData *fringeTileData = Level::GetCurrentFringeTileset()->GetFringeTileDataByID(tileID);
		if (fringeTileData)
		{
			GetComponent<Sprite>()->texture = Assets::RequestTexture(fringeTileData->imageFilename, fringeTileData->filter, fringeTileData->repeatX, fringeTileData->repeatY);
			if (fringeTileData->width == -1 && fringeTileData->height == -1 && GetComponent<Sprite>()->texture)
			{
				GetComponent<Sprite>()->width = GetComponent<Sprite>()->texture->width;
				GetComponent<Sprite>()->height = GetComponent<Sprite>()->texture->height;
			}
			else
			{
				GetComponent<Sprite>()->width = fringeTileData->width;
				GetComponent<Sprite>()->height = fringeTileData->height;
			}

			if (GetComponent<Sprite>()->texture && (GetComponent<Sprite>()->texture->repeatX || GetComponent<Sprite>()->texture->repeatY))
			{
				RefreshScale();
			}
			else if (fringeTileData->atlasX != 0 || fringeTileData->atlasW != 0 || fringeTileData->atlasY != 0 || fringeTileData->atlasH != 0)
			{
				GetComponent<Sprite>()->textureOffset = Vector2(fringeTileData->atlasX / (float)GetComponent<Sprite>()->texture->width, fringeTileData->atlasY / (float)GetComponent<Sprite>()->texture->height);
				GetComponent<Sprite>()->textureScale = Vector2(fringeTileData->atlasW / (float)GetComponent<Sprite>()->texture->width, fringeTileData->atlasH / (float)GetComponent<Sprite>()->texture->height);
			}
		}
	}

	void FringeTile::Save(FileNode *fileNode)
	{
		Entity::Save(fileNode);
		fileNode->Write("tileID", tileID);
		if (GetComponent<Sprite>()->blend != BLEND_ALPHA)
			fileNode->Write("blend", GetComponent<Sprite>()->blend);
	}

	void FringeTile::Load(FileNode *fileNode)
	{
		Entity::Load(fileNode);

		fileNode->Read("tileID", tileID);

		int spriteBlend = BLEND_ALPHA;
		fileNode->Read("blend", spriteBlend);
		GetComponent<Sprite>()->blend = (BlendType)spriteBlend;

		RefreshTexture();
	}

}
