/*This source code copyrighted by Lazy Foo' Productions 2004-2023
and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <iostream>
#include "input.h"
#include "Player.h"
#include "MapLoader.h"
#include "definitions.h"
#include "camera.h"


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
SDL_Texture* woodTexture = NULL;
SDL_Texture* grassTexture = NULL;
SDL_Texture* playerTexture = NULL;



bool init();
void close();
void DrawCircle(SDL_Renderer* renderer, int centreX, int centreY, int radius);



int main(int argc, char* args[])
{
	//Setting up players
	Player* p1 = new Player();
	Player* p2 = new Player();


	// camera coordinates are basically the top left corner of the window, not the center
	SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	// boundingbox of the camera
	Vector2i boundingBox = { 50,50 };

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


		char arr[] = "wood.jpg";
		loadMedia(arr, &woodTexture, gRenderer);
		char arr2[] = "grass.jpg";
		loadMedia(arr2, &grassTexture, gRenderer);
		char arr3[] = "player.png";
		loadMedia(arr3, &playerTexture, gRenderer);


		std::vector<std::string> map = loadMapFromFile("map.txt");

		int targetX = 0;
		float cameraSmooth = 0.93f;
		while (!quit) {

			///////////////////////
			// FRAMETIME COUNTER //
			///////////////////////
			PREVIOUS = CURRENT;
			CURRENT = SDL_GetPerformanceCounter();
			deltaTime = (CURRENT-PREVIOUS) * 1000 / static_cast<float>(SDL_GetPerformanceFrequency());

			/////////////////////
			// CAMERA MOVEMENT //
			/////////////////////
			moveCamera(&camera, p1, boundingBox, &targetX);
			
			/////////////////////////////////////
			// Handle mouse and keyboard input //
			/////////////////////////////////////

			// KEYBOARD (PLAYER 1)
			p1->targetVelocity = { 0,0 };
			int maxSpeed = 10;
			const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
			handleKeyboardInput(currentKeyStates, p1, maxSpeed);
			p1->Move();
			// MOUSE (PLAYER 2)
			bool pushed = false;
			while (SDL_PollEvent(&e) != 0) {
				quit = handleInput(&e, p1,p2);
			}

			
			/////////////////
			// MAP DRAWING //
			/////////////////
			int numberOfColumns = LEVEL_WIDTH / 25;
			int numberOfRows = LEVEL_HEIGHT / 25;
			for (int i = 0; i < numberOfRows; i++) {
				std::string line = map.at(i);
				for (int j = 0; j < numberOfColumns; j++) {
					drawElement(j*25-camera.x, i*25-camera.y, line.at(j), gRenderer,woodTexture,grassTexture);
				}
			}

			//////////////////
			// Draw players //
			//////////////////
			// P1
			drawPlayer(gRenderer, playerTexture, p1->position.x - camera.x, p1->position.y - camera.y);	
			//P2
			DrawCircle(gRenderer, p2->position.x, p2->position.y, 25);

			///////////////////
			// Update screen //
			///////////////////
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
	SDL_DestroyTexture(woodTexture);
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

// probably unnecessary for the player but I will keep it in case I need it later
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



