#pragma once

#include <string>

namespace Monocle
{
	class Component
	{
	public:
		Component();
		virtual ~Component();

		virtual std::string GetName() = 0;

		virtual void Init() = 0;
		virtual void Update() = 0;
		virtual void Unload();
	private:
		bool unloaded;
	};
}