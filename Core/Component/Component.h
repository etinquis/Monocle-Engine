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
		//! Not a virtual copy constructor; this cc will call the Clone method to get a copy of the object.
		Component(const Component& comp);
		virtual ~Component();

		virtual std::string GetName() = 0;
		std::vector<std::string> GetDependencies();

		/*virtual void Init() = 0;*/
		virtual void Update() = 0;
		virtual void Unload();

		virtual Component* Clone() const = 0;
	protected:
		void AddDependency(std::string component_name);
	private:

		bool unloaded;
		std::vector<std::string> dependencies;
	};
}