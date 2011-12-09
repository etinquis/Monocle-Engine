#pragma once

#include "Component.h"

namespace Monocle
{
	class Scene;

	class SceneComponent : public Component
	{
	public:
		SceneComponent();
		virtual ~SceneComponent();

		virtual void Init(Scene* scene);
	protected:
		Scene* scene;
	};
}