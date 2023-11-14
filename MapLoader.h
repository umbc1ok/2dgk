#include <string>
#include <fstream>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>

std::vector<std::string> loadMapFromFile(std::string fileName);
void drawElement(int x, int y, char sign, SDL_Renderer* gRenderer, SDL_Texture* woodTexture, SDL_Texture* grassTexture);
SDL_Surface* loadSurface(std::string path, SDL_Surface* gScreenSurface);
bool loadMedia(char name[], SDL_Texture** gTexture, SDL_Renderer* gRenderer);
SDL_Texture* loadTexture(std::string path, SDL_Renderer* gRenderer);
void drawPlayer(SDL_Renderer* gRenderer, SDL_Texture* playerTexture, int x, int y);

std::vector<std::string> loadMapFromFile(std::string fileName) {
	std::vector<std::string> map;
	std::string line;
	std::ifstream MyReadFile(fileName);
	while (getline(MyReadFile, line)) {
		map.push_back(line);
	}
	MyReadFile.close();
	return map;
}
void drawElement(int x, int y, char sign, SDL_Renderer* gRenderer, SDL_Texture* woodTexture, SDL_Texture* grassTexture) {
	SDL_Rect fillRect = { x, y, 25, 25 };
	if (sign == '=') {
		SDL_SetRenderDrawColor(gRenderer, 173, 216, 230, 0xFF);
		SDL_RenderFillRect(gRenderer, &fillRect);
	}
	else if (sign == 'x') {
		SDL_RenderCopy(gRenderer, grassTexture, NULL, &fillRect);
	}
	else if (sign == 't') {
		SDL_SetRenderDrawColor(gRenderer, 150, 75, 0, 0xFF);
		SDL_RenderFillRect(gRenderer, &fillRect);
	}
	else if (sign == 's') {
		SDL_SetRenderDrawColor(gRenderer, 255, 255, 0, 0xFF);
		SDL_RenderFillRect(gRenderer, &fillRect);
	}
	else if (sign == 'w') {
		SDL_RenderCopy(gRenderer, woodTexture, NULL, &fillRect);
	}
	
}
void drawPlayer(SDL_Renderer* gRenderer, SDL_Texture* playerTexture, int x, int y) {
	SDL_Rect fillRect = { x, y, 50, 50 };
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

