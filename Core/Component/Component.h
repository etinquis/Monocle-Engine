#pragma once

#include <string>
#include <vector>

namespace Monocle
{
	class Game;

	class Component
	{
	public:
		Component();
		virtual ~Component();

		virtual std::string GetName() = 0;
		std::vector<std::string> GetDependencies();

		/*virtual void Init() = 0;*/
		virtual void Update() = 0;
		virtual void Unload();
	protected:
		void AddDependency(std::string component_name);
	private:
		bool unloaded;
		std::vector<std::string> dependencies;
	};
}