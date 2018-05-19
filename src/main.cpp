#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <unistd.h>//for usleep

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

#include "entity.hpp"

#define WINDOW_HEIGHT 300
#define WINDOW_WIDTH 500

#define KEYPRESS_BUFFER_LENGTH 255

#define PLAYER_SPEED 5

void close_all(Display *dis, Window *win, GC *gc, Colormap *cmap);

//game specific data structures

struct key_state{
	uint8_t w: 1;
	uint8_t a: 1;
	uint8_t s: 1;
	uint8_t d: 1;
	uint8_t extra: 4;
};

void init_key_state(struct key_state *ks);

static Entity player("player", "/", 0, 50, 50);

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
	
	//create and initialize key_state
	
	struct key_state keystate;
	init_key_state(&keystate);
	
	
	Display *dis;
	int screen;
	Window win;
	GC gc;//graphics context
	Colormap colormap; 
	
	dis = XOpenDisplay(NULL);
	
	if(dis == NULL){
		std::cerr << "ERROR: Cannot Open Display!\n";
		close_all(dis, &win, &gc, &colormap);
		
	}
	char green_bits[] = "#00FF00";
	char black_bits[] = "#000000";
  	char white_bits[] = "#FFFFFF";
	
	XColor green_col, black_col, white_col;
	colormap = DefaultColormap(dis, 0);
	
	XParseColor(dis, colormap, black_bits, &black_col); 
	XAllocColor(dis, colormap, &black_col);
	XParseColor(dis, colormap, white_bits, &white_col); 
	XAllocColor(dis, colormap, &white_col);
	XParseColor(dis, colormap, green_bits, &green_col); 
	XAllocColor(dis, colormap, &green_col);
	
	
	screen = DefaultScreen(dis);
	
	win = XCreateSimpleWindow(dis, RootWindow(dis, screen), 500, 500, WINDOW_WIDTH, WINDOW_HEIGHT, 1, BlackPixel(dis, screen), WhitePixel(dis, screen));
	XSelectInput(dis, win, StructureNotifyMask | ExposureMask | KeyPressMask | KeyReleaseMask);
	XSetStandardProperties(dis,win,"Behavior Role Playing Game","BRPG", None, NULL, 0, NULL);
	XMapWindow(dis, win);
	
	//wait for the window to "Map," or, appear on the screen
	for(;;){
		XEvent e;
		XNextEvent(dis,&e);
		if(e.type == MapNotify){
			break;
		}
	}
	std::cout << "Press q on the window to exit.\n";
	
	gc = XCreateGC(dis, win, 0, 0);
	
	XEvent event;
	KeySym key;
	char text[KEYPRESS_BUFFER_LENGTH];
	
	uint32_t pos_x = WINDOW_WIDTH / 2;
	uint32_t pos_y = WINDOW_HEIGHT / 2;
	
	const char *village = "Village";
	const char *p_name = "Player";
	while(1){
		
		if(XCheckWindowEvent(dis, win, ExposureMask | KeyPressMask | KeyReleaseMask, &event)){
			
			if (event.type == KeyPress && XLookupString(&event.xkey, text, KEYPRESS_BUFFER_LENGTH, &key, 0) == 1) {
				
				switch(text[0]){
					case('q'):
						close_all(dis, &win, &gc, &colormap);
						break;
					case('w'):
						keystate.w = 1;
						break;
					case('a'):
						keystate.a = 1;
						break;
					case('s'):
						keystate.s = 1;
						break;
					case('d'):
						keystate.d = 1;
						break;
					default:
						break;
					
				}
			
			}
			
			if (event.type == KeyRelease && XLookupString(&event.xkey, text, KEYPRESS_BUFFER_LENGTH, &key, 0) == 1) {
				
				switch(text[0]){
					case('w'):
						keystate.w = 0;
						break;
					case('a'):
						keystate.a = 0;
						break;
					case('s'):
						keystate.s = 0;
						break;
					case('d'):
						keystate.d = 0;
						break;
					default:
						break;
					
				}
			}
			
		}
		
		//update position by normalized velocity (so that going diagonal is same speed as going orthagonal)
		
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
		
		
		XSetForeground(dis, gc, green_col.pixel);
		XFillRectangle(dis,win,gc, pos_x, pos_y , 10, 10);
		
		XSetForeground(dis, gc, black_col.pixel);
		XDrawRectangle(dis,win,gc, pos_x, pos_y, 10, 10);
		XDrawString(dis, win, gc, pos_x, pos_y, p_name, strlen(p_name));
		
		
		XSetForeground(dis, gc, black_col.pixel);
		XDrawString(dis,win,gc, 50, 50, village, strlen(village));
		XFlush(dis);//very necessary
		
		usleep(10*1000);
		
		//clear screen
		XSetForeground(dis, gc, white_col.pixel);
		XFillRectangle(dis,win,gc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	
	
	}
	
	return 0;

}

void init_key_state(struct key_state *ks){
	ks->w = 0;
	ks->a = 0;
	ks->s = 0;
	ks->d = 0;
	ks->extra = 0;
}

void close_all(Display *dis, Window *win, GC *gc, Colormap *cmap){
	XFreeGC(dis, *gc);
	XDestroyWindow(dis, *win);
	XFreeColormap(dis, *cmap);
	XCloseDisplay(dis);
	exit(1);
}