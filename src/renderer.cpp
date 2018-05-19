#include "renderer.hpp"
#include <iostream>

Renderer::Renderer(void){
	
	//create display
	this->_dis = XOpenDisplay(NULL);
	
	//exit if cannot open display
	if(this->_dis == NULL){
		std::cerr << "ERROR Renderer.cpp: Cannot Open Display!\n";
		exit(1);
	}
	
	//create colormap
	this->_colormap = DefaultColormap(this->_dis, 0);
	
	//prepare colors for colormap
	XParseColor(this->_dis, this->_colormap, this->_black_bits, &(this->_black_col)); 
	XAllocColor(this->_dis, this->_colormap, &(this->_black_col));
	XParseColor(this->_dis, this->_colormap, this->_white_bits, &(this->_white_col)); 
	XAllocColor(this->_dis, this->_colormap, &(this->_white_col));
	XParseColor(this->_dis, this->_colormap, this->_green_bits, &(this->_green_col)); 
	XAllocColor(this->_dis, this->_colormap, &(this->_green_col));
	
	//create screen id
	this->_screen = DefaultScreen(this->_dis);
	
	//create window
	this->_win = XCreateSimpleWindow(this->_dis, RootWindow(this->_dis, this->_screen), 500, 500, WINDOW_WIDTH, WINDOW_HEIGHT, 1, BlackPixel(this->_dis, this->_screen), WhitePixel(this->_dis, this->_screen));
	XSelectInput(this->_dis, this->_win, StructureNotifyMask | ExposureMask | KeyPressMask | KeyReleaseMask);
	XSetStandardProperties(this->_dis, this->_win, "Behavior Role Playing Game", "BRPG", None, NULL, 0, NULL);
	XMapWindow(this->_dis, this->_win);
	
	//wait for the window to "Map," or, appear on the screen
	for(;;){
		XEvent e;
		XNextEvent(this->_dis, &e);
		if(e.type == MapNotify){
			break;
		}
	}
	
	//create graphics context
	this->_gc = XCreateGC(this->_dis, this->_win, 0, 0);
	
	//initialize keyboard state
	this->_ks.w = 0;
	this->_ks.a = 0;
	this->_ks.s = 0;
	this->_ks.d = 0;
	this->_ks.extra = 0;
}

Renderer::~Renderer(void){
	XFreeGC(this->_dis, this->_gc);
	XDestroyWindow(this->_dis, this->_win);
	XFreeColormap(this->_dis, this->_colormap);
	XCloseDisplay(this->_dis);
}

void Renderer::drawRectangle(int x, int y, unsigned int width, unsigned int height, XColor color){

	XSetForeground(this->_dis, this->_gc, color.pixel);
	XDrawRectangle(this->_dis, this->_win, this->_gc, x, y , width, height);
	
}

void Renderer::fillRectangle(int x, int y, unsigned int width, unsigned int height, XColor color){
	
	XSetForeground(this->_dis, this->_gc, color.pixel);
	XFillRectangle(this->_dis, this->_win, this->_gc, x, y , width, height);
		
}

void Renderer::drawString(int x, int y, const char *str, XColor color){
	
	XSetForeground(this->_dis, this->_gc, color.pixel);
	XDrawString(this->_dis, this->_win, this->_gc, x, y, str, strlen(str));
}

void Renderer::flush(void){
	XFlush(this->_dis);
}

void Renderer::uSleep(useconds_t usec){
	usleep(usec);
}

XColor Renderer::getColor(const char *name){
	if(strcmp(name, "white") == 0){
		
		return this->_white_col;
	
	}else if(strcmp(name, "black") == 0){
	
		return this->_black_col;
	
	}else{
		
		return this->_green_col;
	}
	
}

struct key_state Renderer::getKeyState(void){

	return this->_ks;
}

void Renderer::updateEvents(void){
	
	XEvent event;
	KeySym key;
	char text[KEYPRESS_BUFFER_LENGTH];
	
	if(XCheckWindowEvent(this->_dis, this->_win, ExposureMask | KeyPressMask | KeyReleaseMask, &event)){

		if (event.type == KeyPress && XLookupString(&event.xkey, text, KEYPRESS_BUFFER_LENGTH, &key, 0) == 1) {

			switch(text[0]){
				case('q'):
					exit(1);
					break;
				case('w'):
					this->_ks.w = 1;
					break;
				case('a'):
					this->_ks.a = 1;
					break;
				case('s'):
					this->_ks.s = 1;
					break;
				case('d'):
					this->_ks.d = 1;
					break;
				default:
					break;

			}

		}

		if (event.type == KeyRelease && XLookupString(&event.xkey, text, KEYPRESS_BUFFER_LENGTH, &key, 0) == 1) {

			switch(text[0]){
				case('w'):
					this->_ks.w = 0;
					break;
				case('a'):
					this->_ks.a = 0;
					break;
				case('s'):
					this->_ks.s = 0;
					break;
				case('d'):
					this->_ks.d = 0;
					break;
				default:
					break;

			}
		}

	}
	
	
}