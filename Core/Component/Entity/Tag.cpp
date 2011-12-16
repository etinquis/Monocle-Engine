#include "Tag.h"

namespace Monocle
{
	Tag::Tag() : tags()
	{
	}

	Tag::Tag(const Tag& tag) : tags(tag.tags)
	{
	}

	Tag::~Tag()
	{
	}

	void Tag::AddTag(const std::string &tag)
	{
		tags.insert(tag);
	}
	
	void Tag::RemoveTag(const std::string &tag)
	{
		tags.erase(tag);
	}

	bool Tag::HasTag(const std::string &tag) const
	{
		return tags.find(tag) != tags.end();
	}
}