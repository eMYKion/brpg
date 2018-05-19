#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <string>
#include <cstdint>

class Entity{
	public:
		Entity(std::string name, std::string category, uint64_t uuid, uint64_t pos_x, uint64_t pos_y);
		~Entity(void);
		std::string getName(void);
		std::string getCategory(void);
		uint64_t getUUID(void);
		uint64_t getPosX(void);
		uint64_t getPosY(void);
	private:
		std::string _name;
		std::string _category;
		uint64_t _uuid;
		uint64_t _pos_x;
		uint64_t _pos_y;
};

#endif