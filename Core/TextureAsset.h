#pragma once

#include "Asset.h"
#include "Vector2.h"

namespace Monocle
{
	class TextureAsset : public Asset
	{
	public:
		TextureAsset();
		void Load(const unsigned char* data, int w, int h, FilterType filter, bool repeatX, bool repeatY);
		void Load(const std::string &filename, FilterType filter, bool repeatX, bool repeatY);
		void UpdateRect(const unsigned char *data, Monocle::Vector2 position, Monocle::Vector2 size);
		void Reload();
		void Unload();

		FilterType filter;
		bool repeatX, repeatY;
		unsigned int width, height;
		unsigned int texID;
	};
}
