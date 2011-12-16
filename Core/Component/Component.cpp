#include "Component.h"

namespace Monocle
{
	Component::Component() : unloaded(false), dependencies()
	{ }

	Component::Component(const Component& comp)
	{
		*this = *comp.Clone();
	}

	Component::~Component()
	{
		if(!unloaded)
		{
			Unload();
		}
	}

	void Component::Unload()
	{
		unloaded = true;
	}

	std::vector<std::string> Component::GetDependencies()
	{
		return dependencies;
	}
}