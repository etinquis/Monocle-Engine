#include "Component.h"

namespace Monocle
{
	Component::Component() : unloaded(false), dependencies()
	{ }

	Component::~Component()
	{
		if(!unloaded)
		{
			Unload();
			unloaded = true;
		}
	}

	void Component::Unload()
	{
		unloaded = true;
	}

	void Component::AddDependency(std::string component_name)
	{
		dependencies.push_back(component_name);
	}

	std::vector<std::string> Component::GetDependencies()
	{
		return dependencies;
	}
}