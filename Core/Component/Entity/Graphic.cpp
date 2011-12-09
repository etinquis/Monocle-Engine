#include "Graphic.h"
#include "../../Vector2.h"

namespace Monocle
{
	Graphic::Graphic()
	{
		AddDependency("Transform");
	}
	Graphic::~Graphic()
	{

	}
}