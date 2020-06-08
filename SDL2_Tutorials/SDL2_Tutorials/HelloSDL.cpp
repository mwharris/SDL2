// Include SDL functions and datatypes
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include "LTexture.h"

// Memory leak detection
#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>  

using std::string;

// Forward declare functions to be used in main();
bool init();
bool loadMedia();
SDL_Texture* loadTexture(string path);
void close();
void setViewport(int x, int y, int w, int h);

// Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Global variables for the window
SDL_Window* g_window = NULL;
SDL_Renderer* g_renderer = NULL;

// Scene Textures
LTexture g_arrowTexture;

int main(int argc, char* args[]) {
	// Initialize SDL and anything else we need
	if (!init()) {
		return -1;
	}
	else {
		if (!loadMedia()) {
			return -1;
		}
		else {
			bool quit = false;
			SDL_Event event;
			double degrees = 0;
			SDL_RendererFlip flipType = SDL_FLIP_NONE;

			// Game loop
			while (!quit) {
				// Event poller
				while (SDL_PollEvent(&event) != 0) {
					if (event.type == SDL_QUIT) {
						quit = true;
					}
					else if (event.type == SDL_KEYDOWN) {
						switch (event.key.keysym.sym) {
							case SDLK_a:
								degrees -= 60;
								break;
							case SDLK_d:
								degrees += 60;
								break;
							case SDLK_q:
								flipType = SDL_FLIP_HORIZONTAL;
								break;
							case SDLK_w:
								flipType = SDL_FLIP_VERTICAL;
								break;
							case SDLK_e:
								flipType = SDL_FLIP_NONE;
								break;
						}
					}
				}
				// Clear the screen
				SDL_SetRenderDrawColor(g_renderer, 255, 255, 255, 255);
				SDL_RenderClear(g_renderer);

				// Render the current animation frame
				int x = (SCREEN_WIDTH - g_arrowTexture.getWidth()) / 2;
				int y = (SCREEN_HEIGHT - g_arrowTexture.getHeight()) / 2;
				g_arrowTexture.render(g_renderer, x, y, NULL, degrees, NULL, flipType);

				// Update the screen
				SDL_RenderPresent(g_renderer);
			}
		}
	}
	// Free up our window, surface, anything else
	close();
	return 0;
}

// TODO: refactor this shit to init both SDL and SDL_Image before setting up the SDL Window
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
			// Create our Renderer to render textures to the screen
			g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (g_renderer == NULL) {
				printf("Error creating the SDL_Renderer! SDL_GetError: %s \n", SDL_GetError());
			}
			else {
				// Set our renderer color for draw operations
				SDL_SetRenderDrawColor(g_renderer, 0, 0, 255, 255);

				// Initialize SDL_image for image loading
				int imgFlags = IMG_INIT_PNG;
				if (IMG_Init(imgFlags) != imgFlags) {
					printf("Error initializing SDL_image! IMG_GetError: %s \n", IMG_GetError());
				}
				else {
					return true;
				}
			}
		}
	}
	return false;
}

// Load all of our images
bool loadMedia() {
	// Load the front texture that will be alpha blended
	if (!g_arrowTexture.loadFromFile(g_renderer, "Images/arrow.png")) {
		printf("Failed to load texture image! \n");
		return false;
	}
	return true;
}

SDL_Texture* loadTexture(string path) {
	// The optimized surface we will return
	SDL_Texture* texture = NULL;
	// Load the image via a string path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) {
		printf("Error while loading file at %s, SDL_GetError: %s \n", path.c_str(), SDL_GetError());
	}
	else {
		// Create a Textures from the Surface we loaded above
		texture = SDL_CreateTextureFromSurface(g_renderer, loadedSurface);
		if (texture == NULL) {
			printf("Error while creating Texture from loaded file at %s, SDL_GetError: %s \n", path.c_str(), SDL_GetError());
		}
		// Free the surface that we loaded
		SDL_FreeSurface(loadedSurface);
		loadedSurface = NULL;
	}
	return texture;
}

void setViewport(int x, int y, int w, int h) {
	SDL_Rect viewportRect;
	viewportRect.x = x;
	viewportRect.y = y;
	viewportRect.w = w;
	viewportRect.h = h;
	SDL_RenderSetViewport(g_renderer, &viewportRect);
}

void close() {
	// Free sprite sheet texture
	g_arrowTexture.free();
	
	// Clean up our SDL_Renderer
	SDL_DestroyRenderer(g_renderer);
	g_renderer = NULL;
	// Clean up our window
	SDL_DestroyWindow(g_window);
	g_window = NULL;

	// Clean up subsystems
	IMG_Quit();
	SDL_Quit();

	_CrtDumpMemoryLeaks();
}