#pragma once

namespace Monocle
{
	class Entity;

	class Entities
	{
		public:
		template<class T>
		void AddEntity()
		{
			Entity* entity = new T();
			buckets[entity->getName()].push_back(entity);
		}
	private:
		std::map<std::string, std::vector<Entity*> > buckets;
	};
}