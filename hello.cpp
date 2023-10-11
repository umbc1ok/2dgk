/*This source code copyrighted by Lazy Foo' Productions 2004-2023
and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <iostream>

//Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//The image we will load and show on the screen
SDL_Surface* loadedSurface = NULL;

bool init();
void close();
SDL_Surface* loadMedia(char name[]);
void putOnScreen(char name[], int x, int y);


enum KeyPressSurfaces
{
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL
};


int main(int argc, char* args[])
{
	//Main loop flag
	bool quit = false;

	//Event handler
	SDL_Event e;

	int imgFlags = IMG_INIT_PNG;
	bool success = true;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		success = false;
	}

	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		int x = 0;
		int y = 0;
		int x2=0, y2=0;
		while (!quit) {
			//Load media
			char name1[] = "circle.png";
			char name2[] = "kwadrat.png";
			while (SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT)
				{
					quit = true;
				}
				else if (e.type == SDL_KEYDOWN)
				{
					//Select surfaces based on key press
					switch (e.key.keysym.sym)
					{
					case SDLK_UP:
						y -= 10;
						break;

					case SDLK_DOWN:
						y += 10;
						break;

					case SDLK_LEFT:
						x -= 10;
						break;

					case SDLK_RIGHT:
						x += 10;
						break;
					default:
						//gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
						break;
					}
				}
				else if (e.type == SDL_MOUSEMOTION) {
					SDL_GetMouseState(&x2, &y2);
				}
			}
			//Apply the image
			SDL_UpdateWindowSurface(gWindow);
			SDL_FillRect(gScreenSurface, NULL, SDL_MapRGB(gScreenSurface->format, 0xFF, 0xFF, 0x00));
			putOnScreen(name1, x, y);
			putOnScreen(name2, x2, y2);
		}

			
	}
	//Free resources and close SDL
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
			//Get window surface
			gScreenSurface = SDL_GetWindowSurface(gWindow);
		}
	}

	return success;
}

void close()
{
	//Deallocate surface
	SDL_FreeSurface(loadedSurface);
	loadedSurface = NULL;

	//Destroy window
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}
SDL_Surface* loadMedia(char name[])
{
	//Loading success flag
	bool success = true;
	SDL_Surface* optimizedSurface = NULL;

	//Load splash image
	loadedSurface = IMG_Load(name);
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL Error: %s\n", name, SDL_GetError());
		success = false;
	}
	else
	{
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
		//Convert surface to screen format
		optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, 0);
		if (optimizedSurface == NULL)
		{
			printf("Unable to optimize image %s! SDL Error: %s\n", name, SDL_GetError());
		}
		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return optimizedSurface;
}

void putOnScreen(char name[], int x, int y)
{
	SDL_Surface* optimizedSurface = loadMedia(name);
	SDL_Rect* dstrect = new SDL_Rect;
	dstrect->x = x;
	dstrect->y = y;
	dstrect->w = optimizedSurface->w;
	dstrect->h = optimizedSurface->h;
	SDL_BlitSurface(optimizedSurface, NULL, gScreenSurface, dstrect);
	delete dstrect;
}

void keepWindowAlive() {
	//Hack to get window to stay up
	SDL_Event e; bool quit = false; while (quit == false) { while (SDL_PollEvent(&e)) { if (e.type == SDL_QUIT) quit = true; } }
}
