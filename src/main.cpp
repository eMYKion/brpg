#include <iostream>
#include <cstdlib>
#include <cstdint>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

#include "entity.hpp"
#include "renderer.hpp"

#define PLAYER_SPEED 5

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
	
	Entity player = Entity("player", "/", 0, 50, 50);
	Renderer renderer = Renderer();
	
	uint32_t pos_x = WINDOW_WIDTH / 2;
	uint32_t pos_y = WINDOW_HEIGHT / 2;
	
	const char *village = "Village";
	const char *p_name = "Player";
	while(1){
		
		//renderer update events
		renderer.updateEvents();
		
		//update position by normalized velocity (so that going diagonal is same speed as going orthagonal)
		key_state keystate = renderer.getKeyState();
		
		if(keystate.d - keystate.a != 0){
			
			if(keystate.s - keystate.w != 0){
				pos_x += PLAYER_SPEED / 1.4142f * (keystate.d - keystate.a);
				pos_y += PLAYER_SPEED / 1.4142f * (keystate.s - keystate.w);
			}else{
				pos_x += PLAYER_SPEED * (keystate.d - keystate.a);	
			}
			
		}else{
		
			if(keystate.s - keystate.w != 0){
				pos_y += PLAYER_SPEED * (keystate.s - keystate.w);
			}else{
				//do nothing
			}
			
		}
		
		renderer.fillRectangle(pos_x, pos_y, 10, 10, renderer.getColor("green"));
		renderer.drawRectangle(pos_x, pos_y, 10, 10, renderer.getColor("black"));
		renderer.drawString(pos_x, pos_y, p_name, renderer.getColor("black"));
		
		renderer.drawString(50, 50, village, renderer.getColor("black"));
		renderer.flush();
		renderer.uSleep(10*1000);
		
		//clear screen
		renderer.fillRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, renderer.getColor("white"));
	
	}
	
	return 0;

}