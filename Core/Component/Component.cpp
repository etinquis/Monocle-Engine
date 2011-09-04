#include "Component.h"

namespace Monocle
{
	Component::Component() : unloaded(false)
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
}