/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/

// Include SDL functions and datatypes
#include <SDL.h>
// Just in order to use printf
#include <stdio.h>

// Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

// Forward declare functions to be used in main();
bool init();
bool loadMedia();
void close();

// Global variables for the window and window surface
SDL_Window* g_window = NULL;
SDL_Surface* g_screenSurface = NULL;
SDL_Surface* g_imageSurface = NULL;

int main(int argc, char* args[])
{
	// Initialize SDL and anything else we need
	if (!init()) {
		return -1;
	}
	else {
		if (!loadMedia()) {
			return -1;
		}
		else {
			// Blit our image to the screen
			SDL_BlitSurface(g_imageSurface, NULL, g_screenSurface, NULL);
			// Tell SDL to draw our updated surface to the screen
			SDL_UpdateWindowSurface(g_window);
			// Wait
			SDL_Delay(3000);
		}
	}

	// Free up our window, surface, anything else
	close();

	return 0;
}

bool init() {
	// We must initialize SDL with SDL_Init before we can use SDL functions
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Error initializing SDL! SDL_GetError: %s \n", SDL_GetError());
	}
	else {
		// Create our SDL Window
		g_window = SDL_CreateWindow("Hello World!",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN);

		// Check our window was created properly
		if (g_window == NULL) {
			printf("Error creating the SDL Window! SDL_GetError: %s \n", SDL_GetError());
		}
		else {
			// Get a reference to the surface our window
			g_screenSurface = SDL_GetWindowSurface(g_window);
			return true;
		}
	}
	return false;
}

bool loadMedia() {
	g_imageSurface = SDL_LoadBMP("Images/bnh.bmp");
	if (g_imageSurface == NULL) {
		printf("Error while loading BMP file! SDL_GetError: %s \n", SDL_GetError());
		return false;
	}
	return true;
}

void close() {
	// Clean up our image surface
	SDL_FreeSurface(g_imageSurface);
	g_imageSurface = NULL;

	// Clean up our window
	SDL_DestroyWindow(g_window);
	g_window = NULL;

	// Clean up SDL
	SDL_Quit();
}