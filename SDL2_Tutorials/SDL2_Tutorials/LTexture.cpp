#include "LTexture.h"

using std::string;

LTexture::LTexture()
{
	texture_ = NULL;
	height_ = 0;
	width_ = 0;
}

LTexture::~LTexture()
{
	free();
}

void LTexture::free()
{
	if (texture_ != NULL)
	{
		SDL_DestroyTexture(texture_);
		texture_ = NULL;
		height_ = 0;
		width_ = 0;
	}
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	int result = SDL_SetTextureColorMod(texture_, red, green, blue);
	if (result < 0) {
		printf("Error while setting texture color modulation, SDL_GetError: %s \n", SDL_GetError());
	}
}

bool LTexture::loadFromFile(SDL_Renderer* renderer, string path)
{
	// Make sure there's no pre-existing texture
	free();
	// The texture we will return
	SDL_Texture* newTexture = NULL;
	
	// Load the image via a string path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) {
		printf("Error while loading file at %s, SDL_GetError: %s \n", path.c_str(), SDL_GetError());
	}
	else {
		// Set the image's color key to our transparent color of choice (cyan)
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 255, 255));

		// Create a Texture from the Surface we loaded above
		newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (newTexture == NULL) {
			printf("Error while creating Texture from loaded file at %s, SDL_GetError: %s \n", path.c_str(), SDL_GetError());
		}
		else {
			// Set image dimensions
			width_ = loadedSurface->w;
			height_ = loadedSurface->h;
		}

		// Free the surface that we loaded
		SDL_FreeSurface(loadedSurface);
		loadedSurface = NULL;
	}
	
	texture_ = newTexture;
	return texture_ != NULL;
}

void LTexture::render(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip)
{
	// Create a rect to render this image at the location given.
	// We don't want to stretch the image, so we pass it our width and height.
	SDL_Rect renderRect = { x, y, width_, height_ };
	// Make sure our source and destination dimensions are the same if we specified a clip
	if (clip != NULL) 
	{
		renderRect.w = clip->w;
		renderRect.h = clip->h;
	}
	// Render source to destination using given Rects
	SDL_RenderCopy(renderer, texture_, clip, &renderRect);
}

int LTexture::getHeight()
{
	return height_;
}

int LTexture::getWidth()
{
	return width_;
}