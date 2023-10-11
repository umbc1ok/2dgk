/*This source code copyrighted by Lazy Foo' Productions 2004-2023
and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <iostream>
#include "input.h"
#include "Player.h"

//Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//The image we will load and show on the screen
SDL_Surface* loadedSurface = NULL;

//Current displayed image
SDL_Surface* gStretchedSurface = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;
//Current displayed texture
SDL_Texture* gTexture = NULL;



bool init();
void close();
bool loadMedia(char name[]);
void putOnScreen(char name[], int x, int y);
SDL_Surface* loadSurface(std::string path);
SDL_Texture* loadTexture(std::string path);



int main(int argc, char* args[])
{
	//Setting up players
	Player* p1 = new Player();
	Player* p2 = new Player();

	//Main loop flag
	bool quit = false;

	//Event handler
	SDL_Event e;
	bool success = true;


	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		while (!quit) {
			//Load media
			char name1[] = "circle.png";
			char name2[] = "kwadrat.png";
			while (SDL_PollEvent(&e) != 0) {
				quit = handleInput(&e, p1,p2);
			}
			//Apply the image
			SDL_RenderClear(gRenderer);
			loadMedia(name1);
			SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
			SDL_RenderPresent(gRenderer);




			/*SDL_UpdateWindowSurface(gWindow);
			SDL_FillRect(gScreenSurface, NULL, SDL_MapRGB(gScreenSurface->format, 0xFF, 0xFF, 0x00));

			loadMedia(name1);
			//Apply the image stretched
			SDL_Rect stretchRect;
			stretchRect.x = p1->x;
			stretchRect.y = p1->y;
			stretchRect.w = 10;
			stretchRect.h = 10;
			SDL_BlitScaled(gStretchedSurface, NULL, gScreenSurface, &stretchRect);
			*/
			
			//putOnScreen(name1, p1->x, p1->y);
			//putOnScreen(name2, p2->x, p2->y);
		}
	}
	close();

	return 0;
}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		success = false;
	}

	return success;
}

void close()
{
	//Deallocate surface
	SDL_DestroyTexture(gTexture);
	loadedSurface = NULL;

	//Destroy window
	SDL_DestroyWindow(gWindow);
	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;
	gWindow = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

SDL_Surface* loadSurface(std::string path) {
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



bool loadMedia(char name[])
{
	//Loading success flag
	bool success = true;

	//Load stretching surface
	gTexture = loadTexture(name);
	if (gTexture == NULL)
	{
		printf("Failed to load stretching image!\n");
		success = false;
	}
	return success;
}

SDL_Texture* loadTexture(std::string path)
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

