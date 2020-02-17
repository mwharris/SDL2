// Include SDL functions and datatypes
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

using std::string;

// Forward declare functions to be used in main();
bool init();
void close();

// Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

// Global variables for the window and window surface
SDL_Window* g_window = NULL;
SDL_Renderer* g_renderer = NULL;


int main(int argc, char* args[]) {
	// Initialize SDL and anything else we need
	if (!init()) {
		return -1;
	}
	else {
		bool quit = false;
		SDL_Event event;

		// Game loop
		while (!quit) {
			// Event poller
			while (SDL_PollEvent(&event) != 0) {
				if (event.type == SDL_QUIT) {
					quit = true;
				}
			}
			// Clear the screen
			SDL_SetRenderDrawColor(g_renderer, 255, 255, 255, 255);
			SDL_RenderClear(g_renderer);

			// Render a red quad
			SDL_Rect fillRect = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
			SDL_SetRenderDrawColor(g_renderer, 255, 0, 0, 255);
			SDL_RenderFillRect(g_renderer, &fillRect);

			// Render a green quad
			fillRect = { SCREEN_WIDTH / 6, SCREEN_HEIGHT / 6, SCREEN_WIDTH * 2 / 3, SCREEN_HEIGHT * 2 / 3 };
			SDL_SetRenderDrawColor(g_renderer, 0, 255, 0, 255);
			SDL_RenderDrawRect(g_renderer, &fillRect);

			// Draw a blue horizontal line
			SDL_SetRenderDrawColor(g_renderer, 0, 0, 255, 255);
			SDL_RenderDrawLine(g_renderer, 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2);

			// Draw a vertical line of yellow dots
			SDL_SetRenderDrawColor(g_renderer, 0, 255, 255, 255);
			for (int i = 0; i < SCREEN_HEIGHT; i += 4) {
				SDL_RenderDrawPoint(g_renderer, SCREEN_WIDTH / 2, i);
			}

			// Update the screen
			SDL_RenderPresent(g_renderer);
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
			g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
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

void close() {
	// Clean up our SDL_Renderer
	SDL_DestroyRenderer(g_renderer);
	g_renderer = NULL;
	// Clean up our window
	SDL_DestroyWindow(g_window);
	g_window = NULL;
	// Clean up SDL_image
	IMG_Quit();
	// Clean up SDL
	SDL_Quit();
}