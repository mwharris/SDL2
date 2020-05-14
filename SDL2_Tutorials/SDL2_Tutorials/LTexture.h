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
	bool loadFromFile(SDL_Renderer* renderer, std::string path);

	// Deallocate memory
	void free();
	
	// Set color modulation
	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	// Set blending
	void setBlendMode(SDL_BlendMode blending);

	void setAplha(Uint8 alpha);

	// Render texture at the given point
	void render(SDL_Renderer* renderer, int x, int y, SDL_Rect* rect = NULL);

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