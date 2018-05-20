#include <iostream>
#include <cstdint>

#include "entity.hpp"
#include "renderer.hpp"

int main(int argc, char **argv){

	//print arguments
	for(int i = 0; i < argc; i++){
		
		char temp = argv[i][0];
		int j = 0;
		
		while(temp != '\0'){
			std::cout << temp;
			temp = argv[i][++j];
		}
		std::cout << "\n";
	}
	
	
	std::cout << "Press q on the window to exit.\n";
	
	Entity player = Entity("player", "/", WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 10, 10, "green");
	Entity stranger = Entity("stranger", "/", WINDOW_WIDTH / 2 + 100, WINDOW_HEIGHT / 2, 20, 10, "white");
	
	World world = World();
	world.entityList.push_back(&player);
	world.entityList.push_back(&stranger);
	
	Renderer renderer = Renderer();
	
	const char *village = "Village";
	
	while(1){
		
		//renderer update events
		renderer.updateEvents();
		
		renderer.updatePlayerPos(player);
				
		renderer.renderWorld(world);
		renderer.drawString(50, 50, village, renderer.getColor("black"));
		renderer.flush();
		
		renderer.uSleep(10*1000);
		
		//clear screen
		renderer.fillRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, renderer.getColor("white"));
	
	}
	
	return 0;

}