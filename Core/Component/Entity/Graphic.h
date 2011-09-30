#pragma once

#include "../EntityComponent.h"

namespace Monocle
{
	class Entity;

	//! Base class for graphic types that may be attached to and rendered with entities.
	class Graphic : public EntityComponent
	{
	public:
		Graphic();
		virtual ~Graphic();

		virtual void Update()=0;
		virtual void Render(Entity *entity)=0;
		virtual void GetWidthHeight(float *width, float *height)=0;
	};
}