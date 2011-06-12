#ifdef MONOCLE_OPENGL_ES

#include "../TTFFontAsset.h"

namespace Monocle
{
	TTFFontAsset::TTFFontAsset() : FontAsset(){ }
    TTFFontAsset::~TTFFontAsset(){}
	bool TTFFontAsset::Load(const std::string &filename, float size, int textureWidth, int textureHeight){ return true; }
	void TTFFontAsset::Reload(){}
	void TTFFontAsset::Unload() {}
	float TTFFontAsset::GetTextWidth(const std::string &text) { return 0; }
	float TTFFontAsset::GetTextHeight(const std::string &text) { return 0; }
	void TTFFontAsset::GetGlyphData(char c, float* x, float* y, Rect& verts, Rect& texCoords) const { }
}

#endif
