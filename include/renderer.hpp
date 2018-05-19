#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <unistd.h>//for usleep
#include <cstring>
#include <cstdint>

#define WINDOW_HEIGHT 300
#define WINDOW_WIDTH 500

#define KEYPRESS_BUFFER_LENGTH 255

struct key_state{
	uint8_t w: 1;
	uint8_t a: 1;
	uint8_t s: 1;
	uint8_t d: 1;
	uint8_t extra: 4;
};

class Renderer{
	public:
		Renderer(void);
		~Renderer(void);
		void updateEvents(void);
	
		void drawRectangle(int x, int y, unsigned int width, unsigned int height, XColor color);
		void fillRectangle(int x, int y, unsigned int width, unsigned int height, XColor color);
	
		void drawString(int x, int y, const char *str, XColor color);
	
		void flush(void);
		void uSleep(useconds_t usec);
		XColor getColor(const char *name);
	
		struct key_state getKeyState(void);
	
	private:
		Display *_dis;
		int _screen;
		Window _win;
		GC _gc;//graphics context
		Colormap _colormap; 
	
		const char *_green_bits = "#00FF00";
		const char *_black_bits = "#000000";
		const char *_white_bits = "#FFFFFF";
	
		XColor _green_col;
		XColor _black_col;
		XColor _white_col;
	
		struct key_state _ks;
};



#endif