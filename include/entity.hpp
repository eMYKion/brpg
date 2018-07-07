#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <string>
#include <cstdint>

class Entity{
	public:
		Entity(std::string name, std::string category, uint64_t pos_x, uint64_t pos_y, uint8_t width, uint8_t height, const char *col);
		~Entity(void);
		const char *getName(void);
		const char *getCategory(void);
		const char *getColorName(void);	
		uint8_t getWidth(void);
		uint8_t getHeight(void);
		
		uint64_t getUUID(void);
		uint64_t getLatestEntityUUID(void);
		uint64_t posX;
		uint64_t posY;
		
	
	private:
		std::string _name;
		std::string _category;
	
		uint64_t _uuid;
		static uint64_t _latest_entity_uuid;
	
		uint8_t _width;
		uint8_t _height;
	
		std::string _color;
		
};

#endif