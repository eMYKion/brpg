#ifndef WORLD_HPP
#define WORLD_HPP

#include <vector>
#include "entity.hpp"

class World{
	public:
		World(void);
		~World();
		std::vector<Entity*> entityList;
		Entity *getEntity(uint64_t uuid);
};

#endif