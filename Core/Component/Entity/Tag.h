#pragma once

#include "../EntityComponent.h"
#include <set>
#include <string>

namespace Monocle
{
	class Tag : public EntityComponent
	{
	public:
		Tag();
		Tag(const Tag& tag);
		virtual ~Tag();

		//! Adds a tag to the tag list.
		//! Ignores an empty string.
		void AddTag(const std::string &tag);
		//! Removes a tag from the tag list.
		//! In the case that there is no matching
		//! tag in the list, nothing will be changed.
		void RemoveTag(const std::string &tag);

		bool HasTag(const std::string &tag) const;
	private:
		std::set<std::string> tags;
	};
}