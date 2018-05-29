#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <cstring>
#include <cstdint>

#include "entity.hpp"
#include "world.hpp"

#define PLAYER_SPEED 5

#define KEYPRESS_BUFFER_LENGTH 255

#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH 1000

#define MAX_HISTORY 17
#define LINE_WIDTH 10
#define TEXT_BOX_MARGIN 2
#define TEXT_BOX_PADDING 2

#define TEXT_BOX_WIDTH WINDOW_WIDTH - 2*TEXT_BOX_MARGIN
#define TEXT_BOX_HEIGHT (MAX_HISTORY + 1) * LINE_WIDTH

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
	
		void drawEntity(Entity &entity);
		void renderWorld(World &world);
	
		void renderTextBox(void);
	
		void updatePlayerPos(Entity &player);
	
		void flush(void);
		XColor getColor(const char *name);
	
		struct key_state getKeyState(void);
	
	private:
		Display *_dis;
		int _screen;
		Window _win;
		GC _gc;//graphics context
		Colormap _colormap; 
	
		bool _text_mode;
		std::string _text_buffer;
		std::vector<std::string> _text_history;
	
		const char *_green_bits = "#00FF00";
		const char *_black_bits = "#000000";
		const char *_white_bits = "#FFFFFF";
	
		XColor _green_col;
		XColor _black_col;
		XColor _white_col;
	
		struct key_state _ks;
};

#endif
