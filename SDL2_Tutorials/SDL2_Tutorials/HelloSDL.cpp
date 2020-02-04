/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/

// Include SDL functions and datatypes
#include <SDL.h>
// Just in order to use printf
#include <stdio.h>

// Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char* args[])
{
	SDL_Window* window;
	SDL_Surface* screen;

	// We must initialize SDL with SDL_Init before we can use SDL functions
	if (SDL_Init(SDL_INIT_VIDEO)) {
		printf("Error initializing SDL! SDL_GetError: %s \n", SDL_GetError());
		return -1;
	}

	// Create our SDL Window
	window = SDL_CreateWindow("Hello World!",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN);

	// Check our window was created properly
	if (window == NULL) {
		printf("Error creating the SDL Window! SDL_GetError: %s \n", SDL_GetError());
	}
	else {
		// Get a reference to the surface our window
		screen = SDL_GetWindowSurface(window);

		// Fill our surface with a color
		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 255, 255));

		// Tell SDL to draw our updated surface to the screen
		SDL_UpdateWindowSurface(window);

		// Wait
		SDL_Delay(2000);
	}

	// Clean up our window
	SDL_DestroyWindow(window);

	// Clean up SDL
	SDL_Quit();

	return 0;
}