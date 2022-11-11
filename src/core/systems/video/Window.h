#pragma once

#include <SDL.h>
#include <memory>

struct WindowConfig {
	int DEFAULT_WIDTH; 
	int DEFAULT_HEIGHT;
	// currently useless

	int width;
	int height;

	int posx;
	int posy;

	int flags;

	WindowConfig();
	~WindowConfig();
	WindowConfig(
		int width, 
		int height, 
		int posx = SDL_WINDOWPOS_CENTERED, 
		int posy = SDL_WINDOWPOS_CENTERED, 
		int flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_GRABBED
	) :
		width(width),
		height(height),
		posx(posx),
		posy(posy),
		flags(flags) {}

	void resolveDefaultConfig();
};

class Window {
private:
	SDL_Window* sdlWindow;

	std::unique_ptr<WindowConfig> config;


public:
	Window(std::unique_ptr<WindowConfig>& config);
	~Window();

	SDL_Window* getSDLWindow();

	int getWidth();
	int getHeight();
};