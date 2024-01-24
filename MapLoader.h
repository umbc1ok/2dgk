#include <string>
#include <fstream>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include "definitions.h"

std::vector<std::string>* loadMapFromFile(std::string fileName);
void drawElement(int x, int y, char sign, SDL_Renderer* gRenderer, SDL_Texture* floorTexture, SDL_Texture* groundTexture, SDL_Texture* skyTexture, SDL_Texture* cloudTexture, SDL_Texture* objectTexture, SDL_Texture* starredTexture);
	SDL_Surface* loadSurface(std::string path, SDL_Surface* gScreenSurface);
bool loadMedia(char name[], SDL_Texture** gTexture, SDL_Renderer* gRenderer);
SDL_Texture* loadTexture(std::string path, SDL_Renderer* gRenderer);
void drawPlayer(SDL_Renderer* gRenderer, SDL_Texture* playerTexture, int x, int y);

std::vector<std::string>* loadMapFromFile(std::string fileName) {
	std::vector<std::string>* map = new std::vector<std::string>();
	std::string line;
	std::ifstream MyReadFile(fileName);
	while (getline(MyReadFile, line)) {
		map->push_back(line);
	}
	MyReadFile.close();
	return map;
}
void drawElement(int x, int y, char sign, SDL_Renderer* gRenderer, SDL_Texture* floorTexture, SDL_Texture* groundTexture, SDL_Texture* skyTexture, SDL_Texture* cloudTexture, SDL_Texture* objectTexture, SDL_Texture* starredTexture) {
	SDL_Rect fillRect = { x, y, TILE_SIZE, TILE_SIZE};
	if (sign == '=') {
		SDL_RenderCopy(gRenderer, floorTexture, NULL, &fillRect);
	}
	else if (sign == '+') {
		SDL_RenderCopy(gRenderer, groundTexture, NULL, &fillRect);
	}
	else if (sign == 'x') {
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
		SDL_RenderFillRect(gRenderer, &fillRect);
	}
	else if (sign == 't') {
		SDL_RenderCopy(gRenderer, skyTexture, NULL, &fillRect);
	}
	else if (sign == 's') {
		SDL_RenderCopy(gRenderer, starredTexture, NULL, &fillRect);
	}
	else if (sign == '1') {
		SDL_RenderCopy(gRenderer, cloudTexture, NULL, &fillRect);
		//SDL_SetRenderDrawColor(gRenderer, 255, 255, 0, 0xFF);
		//SDL_RenderFillRect(gRenderer, &fillRect);
	}
	else if (sign == '2') {
		fillRect.w = TILE_SIZE * 2;
		fillRect.h = TILE_SIZE * 2;
		SDL_RenderCopy(gRenderer, cloudTexture, NULL, &fillRect);
		//SDL_SetRenderDrawColor(gRenderer, 255, 255, 0, 0xFF);
		//SDL_RenderFillRect(gRenderer, &fillRect);
	}
	else if (sign == '3') {
		fillRect.w = TILE_SIZE * 3;
		fillRect.h = TILE_SIZE * 3;
		SDL_RenderCopy(gRenderer, cloudTexture, NULL, &fillRect);
		//SDL_SetRenderDrawColor(gRenderer, 255, 255, 0, 0xFF);
		//SDL_RenderFillRect(gRenderer, &fillRect);
	}
	else if (sign == '4') {
		fillRect.w = TILE_SIZE * 4;
		fillRect.h = TILE_SIZE * 4;
		SDL_RenderCopy(gRenderer, cloudTexture, NULL, &fillRect);
		//SDL_SetRenderDrawColor(gRenderer, 255, 255, 0, 0xFF);
		//SDL_RenderFillRect(gRenderer, &fillRect);
	}
	else if (sign == 'w') {
		fillRect.w = TILE_SIZE * 5;
		fillRect.h = TILE_SIZE * 5;
		SDL_RenderCopy(gRenderer, objectTexture, NULL, &fillRect);
	}
	
}
void drawPlayer(SDL_Renderer* gRenderer, SDL_Texture* playerTexture, int x, int y) {
	SDL_Rect fillRect = { x, y, 64, 64 };
	SDL_RenderCopy(gRenderer, playerTexture, NULL, &fillRect);
}




SDL_Surface* loadSurface(std::string path, SDL_Surface* gScreenSurface) {
	//The final optimized image
	SDL_Surface* optimizedSurface = NULL;

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
	}
	else
	{
		//Convert surface to screen format
		optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, 0);
		if (optimizedSurface == NULL)
		{
			printf("Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return optimizedSurface;
}



bool loadMedia(char name[], SDL_Texture** gTexture, SDL_Renderer* gRenderer)
{
	//Loading success flag
	bool success = true;

	//Load stretching surface
	*gTexture = loadTexture(name, gRenderer);
	if (gTexture == NULL)
	{
		printf("Failed to load stretching image!\n");
		success = false;
	}
	return success;
}

SDL_Texture* loadTexture(std::string path, SDL_Renderer* gRenderer)
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}
	return newTexture;
}

