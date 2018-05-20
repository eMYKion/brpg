#include "entity.hpp"

uint64_t Entity::_latest_entity_uuid = 0; 

Entity::Entity(std::string name, std::string category, uint64_t pos_x, uint64_t pos_y, uint8_t width, uint8_t height, const char *col){
	this->_name = name;
	this->_category = category;
	
	_latest_entity_uuid++;
	this->_uuid = _latest_entity_uuid;
	
	this->posX = pos_x;
	this->posY = pos_y;
	
	this->_width = width;
	this->_height = height;
	this->_color = std::string(col);
}

Entity::~Entity(void){
	
}

const char *Entity::getName(void){
	return this->_name.c_str();
}

const char *Entity::getCategory(void){
	return this->_category.c_str();
}

uint64_t Entity::getUUID(void){
	return this->_uuid;
}

uint64_t Entity::getLatestEntityUUID(void){
	return _latest_entity_uuid;
}

const char *Entity::getColorName(void){
	return this->_color.c_str();
}

uint8_t Entity::getWidth(void){
	return this->_width;
}

uint8_t Entity::getHeight(void){
	return this->_height;
}

