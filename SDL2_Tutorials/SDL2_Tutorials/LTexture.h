#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>

class LTexture
{
public:
	LTexture();
	~LTexture();

	// Load image at specified path
	bool loadFromFile(std::string path, SDL_Renderer* renderer);

	// Deallocate memory
	void free();

	// Render texture at the given point
	void render(int x, int y, SDL_Renderer* renderer);

	// Get image dimensions
	int getWidth();
	int getHeight();

private:
	// The texture we are wrapping
	SDL_Texture* texture_;

	// Dimensions of the texture
	int width_;
	int height_;
};