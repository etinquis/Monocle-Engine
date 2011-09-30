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
		int spriteBlend = (int)((Sprite*)(*this)["Sprite"])->blend;
		spriteBlend++;
		((Sprite*)(*this)["Sprite"])->blend = (BlendType)spriteBlend;
	}

	void FringeTile::PrevBlend()
	{
		int spriteBlend = (int)((Sprite*)(*this)["Sprite"])->blend;
		spriteBlend--;
		((Sprite*)(*this)["Sprite"])->blend = (BlendType)spriteBlend;
	}

	void FringeTile::Update()
	{
		Entity::Update();
		
		RefreshScale();
	}

	void FringeTile::RefreshScale()
	{
		if (((Sprite*)(*this)["Sprite"])->texture && (((Sprite*)(*this)["Sprite"])->texture->repeatX || ((Sprite*)(*this)["Sprite"])->texture->repeatY))
		{
			const FringeTileData *fringeTileData = Level::GetCurrentFringeTileset()->GetFringeTileDataByID(tileID);
			if (fringeTileData->autoTile)
			{
				if (((Sprite*)(*this)["Sprite"])->texture->repeatX)
				{
					((Sprite*)(*this)["Sprite"])->textureScale.x = ((Transform*)(*this)["Transform"])->scale.x;
				}
				if (((Sprite*)(*this)["Sprite"])->texture->repeatY)
				{
					((Sprite*)(*this)["Sprite"])->textureScale.y = ((Transform*)(*this)["Transform"])->scale.y;
				}
			}

		}
	}

	void FringeTile::RefreshTexture()
	{
		//printf("RefreshTexture to tileID: %d\n", tileID);

		// free old texture here somehow:
		if (((Sprite*)(*this)["Sprite"])->texture)
		{
			((Sprite*)(*this)["Sprite"])->texture->RemoveReference();
			((Sprite*)(*this)["Sprite"])->texture = NULL;
		}

		const FringeTileData *fringeTileData = Level::GetCurrentFringeTileset()->GetFringeTileDataByID(tileID);
		if (fringeTileData)
		{
			((Sprite*)(*this)["Sprite"])->texture = Assets::RequestTexture(fringeTileData->imageFilename, fringeTileData->filter, fringeTileData->repeatX, fringeTileData->repeatY);
			if (fringeTileData->width == -1 && fringeTileData->height == -1 && ((Sprite*)(*this)["Sprite"])->texture)
			{
				((Sprite*)(*this)["Sprite"])->width = ((Sprite*)(*this)["Sprite"])->texture->width;
				((Sprite*)(*this)["Sprite"])->height = ((Sprite*)(*this)["Sprite"])->texture->height;
			}
			else
			{
				((Sprite*)(*this)["Sprite"])->width = fringeTileData->width;
				((Sprite*)(*this)["Sprite"])->height = fringeTileData->height;
			}

			if (((Sprite*)(*this)["Sprite"])->texture && (((Sprite*)(*this)["Sprite"])->texture->repeatX || ((Sprite*)(*this)["Sprite"])->texture->repeatY))
			{
				RefreshScale();
			}
			else if (fringeTileData->atlasX != 0 || fringeTileData->atlasW != 0 || fringeTileData->atlasY != 0 || fringeTileData->atlasH != 0)
			{
				((Sprite*)(*this)["Sprite"])->textureOffset = Vector2(fringeTileData->atlasX / (float)((Sprite*)(*this)["Sprite"])->texture->width, fringeTileData->atlasY / (float)((Sprite*)(*this)["Sprite"])->texture->height);
				((Sprite*)(*this)["Sprite"])->textureScale = Vector2(fringeTileData->atlasW / (float)((Sprite*)(*this)["Sprite"])->texture->width, fringeTileData->atlasH / (float)((Sprite*)(*this)["Sprite"])->texture->height);
			}
		}
	}

	void FringeTile::Save(FileNode *fileNode)
	{
		Entity::Save(fileNode);
		fileNode->Write("tileID", tileID);
		if (((Sprite*)(*this)["Sprite"])->blend != BLEND_ALPHA)
			fileNode->Write("blend", (int)((Sprite*)(*this)["Sprite"])->blend);
	}

	void FringeTile::Load(FileNode *fileNode)
	{
		Entity::Load(fileNode);

		fileNode->Read("tileID", tileID);

		int spriteBlend = BLEND_ALPHA;
		fileNode->Read("blend", spriteBlend);
		((Sprite*)(*this)["Sprite"])->blend = (BlendType)spriteBlend;

		RefreshTexture();
	}

}
