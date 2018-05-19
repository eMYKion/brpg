#include "entity.hpp"

Entity::Entity(std::string name, std::string category, uint64_t uuid, uint64_t pos_x, uint64_t pos_y){
	this->_name = name;
	this->_category = category;
	this->_uuid = uuid;
	this->posX = pos_x;
	this->posY = pos_y;
}

Entity::~Entity(void){
	
}

std::string Entity::getName(void){
	return this->_name;
}

std::string Entity::getCategory(void){
	return this->_category;
}

uint64_t Entity::getUUID(void){
	return this->_uuid;
}

