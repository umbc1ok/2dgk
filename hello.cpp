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
void DrawCircle(SDL_Renderer* renderer, int centreX, int centreY, int radius);



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
		Uint64 PREVIOUS = 0;
		Uint64 CURRENT = 0;
		Uint64 deltaTime;
		Uint64 desiredFrameTime = 17; // 1/60 of a second
		while (!quit) {

			//counting frame time
			PREVIOUS = CURRENT;
			CURRENT = SDL_GetPerformanceCounter();
			deltaTime = (CURRENT-PREVIOUS) * 1000 / static_cast<float>(SDL_GetPerformanceFrequency());


			//Load media
			char name1[] = "circle.png";
			char name2[] = "kwadrat.png";
			bool pushed = false;
			while (SDL_PollEvent(&e) != 0) {
				quit = handleInput(&e, p1,p2);
			}
			p1->targetVelocity = { 0,0 };
			int maxSpeed = 2;
			const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
			if (currentKeyStates[SDL_SCANCODE_UP])
			{
				p1->targetVelocity.y = -maxSpeed;
			}

			if (currentKeyStates[SDL_SCANCODE_DOWN])
			{
				p1->targetVelocity.y = maxSpeed;
			}

			if (currentKeyStates[SDL_SCANCODE_LEFT])
			{
				p1->targetVelocity.x = -maxSpeed;
			}

			if (currentKeyStates[SDL_SCANCODE_RIGHT])
			{
				p1->targetVelocity.x = maxSpeed;
			}
			if (currentKeyStates[SDL_SCANCODE_UP] && currentKeyStates[SDL_SCANCODE_LEFT])
			{
				p1->targetVelocity.y = -maxSpeed;
				p1->targetVelocity.x = -maxSpeed;
			}
			if (currentKeyStates[SDL_SCANCODE_UP] && currentKeyStates[SDL_SCANCODE_RIGHT])
			{
				p1->targetVelocity.y = -maxSpeed;
				p1->targetVelocity.x = maxSpeed;
			}
			if (currentKeyStates[SDL_SCANCODE_DOWN] && currentKeyStates[SDL_SCANCODE_LEFT])
			{
				p1->targetVelocity.y = maxSpeed;
				p1->targetVelocity.x = -maxSpeed;
			}
			if (currentKeyStates[SDL_SCANCODE_DOWN] && currentKeyStates[SDL_SCANCODE_RIGHT])
			{
				p1->targetVelocity.y = maxSpeed;
				p1->targetVelocity.x = maxSpeed;
			}
			
			p1->Move(deltaTime);

			//Apply the image
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(gRenderer);


			//Render red filled quad
			SDL_Rect fillRect = { p1->position.x, p1->position.y, 50, 50 };
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
			SDL_RenderFillRect(gRenderer, &fillRect);

			DrawCircle(gRenderer, p2->position.x, p2->position.y, 25);

			//Update screen
			SDL_RenderPresent(gRenderer);
			if (desiredFrameTime > deltaTime) // If the desired frame delay is greater than the deltaTime
			{
				SDL_Delay(desiredFrameTime - deltaTime); // Delay for the remaining time
			}


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
				// set blend mode so that alpha values matter
				SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
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
void DrawCircle(SDL_Renderer* renderer, int x, int y, int radius)
{
	SDL_Color color;
	color.r = 100;
	color.g = 50;
	color.b = 30;
	color.a = 100;
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	for (int w = 0; w < radius * 2; w++)
	{
		for (int h = 0; h < radius * 2; h++)
		{
			int dx = radius - w; // horizontal offset
			int dy = radius - h; // vertical offset
			if ((dx * dx + dy * dy) <= (radius * radius))
			{
				SDL_RenderDrawPoint(renderer, x + dx, y + dy);
			}
		}
	}
}



