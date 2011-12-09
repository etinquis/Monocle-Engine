#include "SceneComponent.h"

namespace Monocle
{
	SceneComponent::SceneComponent() : Component()
	{

	}

	SceneComponent::~SceneComponent()
	{

	}

	void SceneComponent::Init(Scene *scene)
	{
		this->scene = scene;
	}
}