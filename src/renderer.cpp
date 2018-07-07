#include "renderer.hpp"
#include <iostream>
#include <algorithm>

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
	
	this->_text_mode = false;
	this->_text_buffer = "";
}

Renderer::~Renderer(void){
	XFreeGC(this->_dis, this->_gc);
	XDestroyWindow(this->_dis, this->_win);
	XFreeColormap(this->_dis, this->_colormap);
	XCloseDisplay(this->_dis);
	this->_text_buffer = "";
	this->_text_history.clear();
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

void Renderer::drawEntity(Entity &entity){
	XSetForeground(this->_dis, this->_gc, this->getColor(entity.getColorName()).pixel);
	XFillRectangle(this->_dis, this->_win, this->_gc, entity.posX, entity.posY, entity.getWidth(), entity.getHeight());
	XSetForeground(this->_dis, this->_gc, this->getColor("black").pixel);
	XDrawRectangle(this->_dis, this->_win, this->_gc, entity.posX, entity.posY, entity.getWidth(), entity.getHeight());
	
	const char* name = entity.getName();
	XDrawString(this->_dis, this->_win, this->_gc, entity.posX, entity.posY, name, strlen(name));
}

void Renderer::renderTextBox(void){
	
	XSetForeground(this->_dis, this->_gc, this->getColor("black").pixel);
	XDrawRectangle(this->_dis, this->_win, this->_gc, TEXT_BOX_MARGIN, WINDOW_HEIGHT - TEXT_BOX_HEIGHT - TEXT_BOX_MARGIN, TEXT_BOX_WIDTH , TEXT_BOX_HEIGHT);
	
	if(this->_text_mode){
		
		XDrawString(this->_dis, this->_win, this->_gc, 5 + TEXT_BOX_MARGIN, WINDOW_HEIGHT - TEXT_BOX_HEIGHT - 2 * TEXT_BOX_MARGIN, "text mode on", strlen("text mode on"));
		
	}
	
	for(int i = 0, n = std::min((int)this->_text_history.size(), MAX_HISTORY); i < n; i++){
		
		const char *msg = this->_text_history.at(std::max((int)this->_text_history.size() - MAX_HISTORY, 0) + n -1 - i).c_str();
		
		XDrawString(this->_dis, this->_win, this->_gc, TEXT_BOX_MARGIN,  WINDOW_HEIGHT - TEXT_BOX_MARGIN - LINE_WIDTH - LINE_WIDTH * i, msg, strlen(msg));
		
	}
	
	XDrawString(this->_dis, this->_win, this->_gc, TEXT_BOX_MARGIN,  WINDOW_HEIGHT - TEXT_BOX_MARGIN, this->_text_buffer.c_str(), strlen(this->_text_buffer.c_str()));
	
}

void Renderer::renderWorld(World &world){

	for(int i = 0, n = world.entityList.size(); i < n; i++ ){
		if(world.entityList.at(i)->posX >= 0 && world.entityList.at(i)->posX <= WINDOW_WIDTH && world.entityList.at(i)->posY >= 0 && world.entityList.at(i)->posY <= WINDOW_HEIGHT - TEXT_BOX_HEIGHT){
			this->drawEntity(*(world.entityList.at(i)));
		}
		
	}
}

void Renderer::updatePlayerPos(Entity &player){
	
	//update position by normalized velocity (so that going diagonal is same speed as going orthagonal)
	
	if(this->_ks.d - this->_ks.a != 0){

		if(this->_ks.s - this->_ks.w != 0){
			player.posX += PLAYER_SPEED / 1.4142f * (this->_ks.d - this->_ks.a);
			player.posY += PLAYER_SPEED / 1.4142f * (this->_ks.s - this->_ks.w);
		}else{
			player.posX += PLAYER_SPEED * (this->_ks.d - this->_ks.a);	
		}

	}else{

		if(this->_ks.s - this->_ks.w != 0){
			
			player.posY += PLAYER_SPEED * (this->_ks.s - this->_ks.w);
			
		}else{
			//do nothing
		}

	}

}

void Renderer::flush(void){
	XFlush(this->_dis);
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
		
		if(! this->_text_mode){

			if (event.type == KeyPress && XLookupString(&event.xkey, text, KEYPRESS_BUFFER_LENGTH, &key, 0) == 1) {

				switch(text[0]){
					case('/'):
						this->_text_mode = true;
						break;
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
			
		}else{//if not in text mode
			
			if (event.type == KeyPress && XLookupString(&event.xkey, text, KEYPRESS_BUFFER_LENGTH, &key, 0) == 1){
				
				if(text[0] == '/'){
					this->_text_mode = false;
				}else{
					
					switch(text[0]){
						
						case('\r'):
							this->_text_history.push_back(this->_text_buffer);
							this->_text_buffer = "";
							break;
							
						case('\b'):
							if(this->_text_buffer.length() > 0){
								this->_text_buffer.pop_back();
							}
							break;
						default:
							this->_text_buffer.append(1, text[0]);
							break;
					}
				
				
					//TODO:: add protections against breaking inputs
				
				}
			}
		
		}

	}
	
	
}
