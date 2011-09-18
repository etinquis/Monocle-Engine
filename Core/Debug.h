#pragma once

#include "Component/GameComponent.h"
#include "Vector2.h"
#include "Vector3.h"
#include <string>

namespace Monocle
{
	class Entity;

	//!
	//! \brief Holds debug log functions, stores data that controls debug output throughout Monocle
	//!
	class Debug : public GameComponent
	{
	public:
		virtual std::string GetName() { return "Debug"; }

		virtual void Init(Game* game);
		virtual void Update();
		virtual void Unload();

		static void Log(const char *string);
		static void Log(bool boolean);
		static void Log(int num);
		static void Log(long num);
		static void Log(float num);
		static void Log(double num);
		static void Log(const Vector2& vec);
		static void Log(const Vector3& vec);
		static void Log(const std::string& string);

		Debug *Clone() const;

		bool render;
		bool showBounds;

		//! used by LevelEditor and PuppetEditor, as well as Entity for rendering selection effects
		Entity *selectedEntity;

		//! Entities in layers outside of this range won't be subject to Debug::render or Debug::showBounds
		int layerMin;
		int layerMax;
	};
}