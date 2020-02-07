// Include SDL functions and datatypes
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

using std::string;

// Forward declare functions to be used in main();
bool init();
bool loadMedia();
void close();
SDL_Surface* loadSurface(string path);

// Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

// Key press value to surface constants
enum KeyPressSurfaces {
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL
};

// Global variables for the window and window surface
SDL_Window* g_window = NULL;
SDL_Surface* g_screenSurface = NULL;
SDL_Surface* g_currentSurface = NULL;

// Surfaces tied to key press inputs
SDL_Surface* g_keyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];


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

			// Set our default image
			g_currentSurface = g_keyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];

			// Game loop
			while (!quit) {				
				// Event poller
				while (SDL_PollEvent(&event) != 0) {
					if (event.type == SDL_QUIT) {
						quit = true;
					}
					else if (event.type == SDL_KEYDOWN) {
						// Change our shown image based on keyboard input
						switch (event.key.keysym.sym)
						{
						case SDLK_UP:
							g_currentSurface = g_keyPressSurfaces[KEY_PRESS_SURFACE_UP];
							break;
						case SDLK_LEFT:
							g_currentSurface = g_keyPressSurfaces[KEY_PRESS_SURFACE_LEFT];
							break;
						case SDLK_RIGHT:
							g_currentSurface = g_keyPressSurfaces[KEY_PRESS_SURFACE_RIGHT];
							break;
						case SDLK_DOWN:
							g_currentSurface = g_keyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
							break;
						default:
							g_currentSurface = g_keyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
							break;
						}
					}
				}
				// Blit our current image to the screen
				SDL_BlitScaled(g_currentSurface, NULL, g_screenSurface, NULL);
				// Tell SDL to draw our updated surface to the screen
				SDL_UpdateWindowSurface(g_window);
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
			// Initialize SDL_image for image loading
			int imgFlags = IMG_INIT_PNG;
			if (IMG_Init(imgFlags) != imgFlags) {
				printf("Error initializing SDL_image! IMG_GetError: %s \n", IMG_GetError());
			}
			else {
				// Get a reference to the surface our window
				g_screenSurface = SDL_GetWindowSurface(g_window);
				return true;
			}
		}
	}
	return false;
}

// Load all of our images
bool loadMedia() {
	g_keyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] = loadSurface("Images/bnh_default.png");
	if (g_keyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] == NULL) {
		printf("Failed to load default image! \n");
		return false;
	}

	g_keyPressSurfaces[KEY_PRESS_SURFACE_UP] = loadSurface("Images/bnh_up.png");
	if (g_keyPressSurfaces[KEY_PRESS_SURFACE_UP] == NULL) {
		printf("Failed to load up image! \n");
		return false;
	}

	g_keyPressSurfaces[KEY_PRESS_SURFACE_LEFT] = loadSurface("Images/bnh_left.png");
	if (g_keyPressSurfaces[KEY_PRESS_SURFACE_LEFT] == NULL) {
		printf("Failed to load left image! \n");
		return false;
	}

	g_keyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] = loadSurface("Images/bnh_right.png");
	if (g_keyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] == NULL) {
		printf("Failed to load right image! \n");
		return false;
	}

	g_keyPressSurfaces[KEY_PRESS_SURFACE_DOWN] = loadSurface("Images/bnh_down.png");
	if (g_keyPressSurfaces[KEY_PRESS_SURFACE_DOWN] == NULL) {
		printf("Failed to load down image! \n");
		return false;
	}

	return true;
}

SDL_Surface* loadSurface(string path) {
	// The optimized surface we will return
	SDL_Surface* optimizedSurface = NULL;
	// Load the image BMP via a string path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) {
		printf("Error while loading BMP file at %s, SDL_GetError: %s \n", path.c_str(), SDL_GetError());
	}
	else {
		// Optimize the loaded surface for our screen
		optimizedSurface = SDL_ConvertSurface(loadedSurface, g_screenSurface->format, 0);
		if (optimizedSurface == NULL) {
			printf("Error while optimizing BMP file at %s, SDL_GetError: %s \n", path.c_str(), SDL_GetError());
		}
		// Free the original surface
		SDL_FreeSurface(loadedSurface);
		loadedSurface = NULL;
	}
	return optimizedSurface;
}

void close() {
	// Clean up our key press surfaces
	for (int i = 0; i < KEY_PRESS_SURFACE_TOTAL; i++) {
		if (g_keyPressSurfaces[i] != NULL) {
			SDL_FreeSurface(g_keyPressSurfaces[i]);
			g_keyPressSurfaces[i] = NULL;
		}
	}
	// Clean up our window
	SDL_DestroyWindow(g_window);
	g_window = NULL;
	// Clean up SDL_image
	IMG_Quit();
	// Clean up SDL
	SDL_Quit();
}