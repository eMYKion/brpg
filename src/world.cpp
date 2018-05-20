#include "world.hpp"

World::World(void){

}

World::~World(){
	this->entityList.clear();//make sure if this is destroying reference or copy of entities 
}

Entity *World::getEntity(uint64_t uuid){
	if(uuid < this->entityList.size() && uuid == this->entityList.at(uuid)->getUUID()){
		return this->entityList.at(uuid);
	}else{
		for(uint64_t i = 0, n = this->entityList.size(); i < n; i++){
			if(uuid == this->entityList.at(uuid)->getUUID()){
				return this->entityList.at(uuid);
			}
		}	
	}
	
	return (Entity *)nullptr;
}