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
#include "collisions.h"
#include "scoreboard.h"


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
SDL_Texture* player2Texture = NULL;



bool init();
void close();
void DrawCircle(SDL_Renderer* renderer, int centreX, int centreY, int radius);
void DrawQuad(SDL_Renderer* renderer, int x, int y, int width, int height);
void DrawDottedLine(SDL_Renderer* renderer, int x0, int y0, int x1, int y1);
Vector2f randomizeTreasure(std::vector<std::string>* map);



int main(int argc, char* args[])
{
	srand(time(NULL));
	//Setting up players
	// kwadrat
	Player* p1 = new Player();
	// kolo
	Player* p2 = new Player();

	Scoreboard* scoreboard = new Scoreboard();


	p1->radius = 15;
	p2->radius = 15;
	std::vector<Vector2i> spawnPoints1;
	std::vector<Vector2i> spawnPoints2;
	std::vector<Vector2i> spawnPoints3;

	spawnPoints1.push_back({ 70,120 });
	spawnPoints1.push_back({ 120,70 });


	spawnPoints1.push_back({ 1280,520 });
	spawnPoints1.push_back({ 1110,520 });

	spawnPoints1.push_back({ 570,220 });
	spawnPoints1.push_back({ 1110,170 });




	spawnPoints2.push_back({ 610,420 });
	spawnPoints2.push_back({ 580,720 });

	spawnPoints2.push_back({ 75,720 });
	spawnPoints2.push_back({ 170,720 });

	spawnPoints2.push_back({ 70,120 });
	spawnPoints2.push_back({ 110,120 });

	spawnPoints2.push_back({ 570,210 });
	spawnPoints2.push_back({ 1110,170 });



	spawnPoints3.push_back({1470,850 });
	spawnPoints3.push_back({ 1620,760 });

	spawnPoints3.push_back({ 610,420 });
	spawnPoints3.push_back({ 580,720 });

	spawnPoints3.push_back({ 75,720 });
	spawnPoints3.push_back({ 170,720 });

	spawnPoints3.push_back({ 70,120 });
	spawnPoints3.push_back({ 110,80 });

	spawnPoints3.push_back({ 570,220 });
	spawnPoints3.push_back({ 1110,170 });





	p1->position = spawnPoints1.at(0);
	p2->position = spawnPoints1.at(1);






	// camera coordinates are basically the top left corner of the window, not the center
	SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
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
		char arr4[] = "player2.png";
		loadMedia(arr4, &player2Texture, gRenderer);


		std::vector<std::string>* map = loadMapFromFile("map2.txt");

		Vector2f treasureTile = randomizeTreasure(map);




		int targetX = 0;
		float cameraSmooth = 0.93f;
		bool collisions = true;
		bool separation = true;
		int currentMap = 0;

		while (!quit) {
			if (currentMap != scoreboard->checkCurrentMap()) {
				currentMap = scoreboard->checkCurrentMap();
				p1->position = { rand() % 750 + 50,rand() % 500 + 50 };
				p2->position = { rand() % 750 + 50,rand() % 500 + 50 };
				std::cout << currentMap << std::endl;
				if (currentMap == 0) {
					map = loadMapFromFile("map2.txt");
					int positionSetIndex = rand() % (spawnPoints1.size()/2);
					p1->position = spawnPoints1.at(positionSetIndex*2);
					p2->position = spawnPoints1.at(positionSetIndex*2 + 1);
				}
				else if (currentMap == 1) {
					map = loadMapFromFile("map3.txt");
					int positionSetIndex = rand() % (spawnPoints2.size()/2);
					p1->position = spawnPoints2.at(positionSetIndex*2);
					p2->position = spawnPoints2.at(positionSetIndex*2 + 1);
				}
				else if (currentMap == 2) {
					map = loadMapFromFile("map4.txt");
					int positionSetIndex = rand() % (spawnPoints3.size()/2);
					p1->position = spawnPoints3.at(positionSetIndex*2);
					p2->position = spawnPoints3.at(positionSetIndex*2 + 1);
				}
				treasureTile = randomizeTreasure(map);
			}
			///////////////////////
			// FRAMETIME COUNTER //
			///////////////////////
			PREVIOUS = CURRENT;
			CURRENT = SDL_GetPerformanceCounter();
			deltaTime = (CURRENT-PREVIOUS) * 1000 / static_cast<float>(SDL_GetPerformanceFrequency());

			/////////////////////
			// CAMERA MOVEMENT //
			/////////////////////

			moveCamera(&camera, p1, p2, boundingBox, &targetX);
			
			/////////////////////////////////////
			// Handle mouse and keyboard input //
			/////////////////////////////////////

			// KEYBOARD (PLAYER 1)
			p1->targetVelocity = { 0,0 };
			p2->targetVelocity = { 0,0 };
			int maxSpeed = 3;
			const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
			handleKeyboardInput(currentKeyStates, p1,p2,maxSpeed);


			bool pushed = false;
			while (SDL_PollEvent(&e) != 0) {
				quit = handleInput(&e, p1,p2);
			}

			
			/////////////////
			// MAP DRAWING //
			/////////////////
			int numberOfColumns = LEVEL_WIDTH / TILE_SIZE;
			int numberOfRows = LEVEL_HEIGHT / TILE_SIZE;
			for (int i = 0; i < numberOfRows; i++) {
				std::string line = map->at(i);
				for (int j = 0; j < numberOfColumns; j++) {
					drawElement(j* TILE_SIZE -camera.x, i* TILE_SIZE -camera.y, line.at(j), gRenderer,woodTexture,grassTexture);
				}
			}



			
			p1->checkCollision(*p2);
			p1->separate(*p2);
			p2->checkCollision(*p1);
			p2->separate(*p1);
			p1->MoveX();
			if(collideWithLabirynthWalls(p1, *map, false, scoreboard)){
				p1->position.x -= int(round(p1->velocity.x));
			}
			p1->MoveY();
			if (collideWithLabirynthWalls(p1, *map, false, scoreboard)) {
				p1->position.y -= int(round(p1->velocity.y));
			}
			p2->MoveX();
			if (collideWithLabirynthWalls(p2, *map, true, scoreboard)) {
				p2->position.x -= int(round(p2->velocity.x));
			}
			p2->MoveY();
			if (collideWithLabirynthWalls(p2, *map, true, scoreboard)) {
				p2->position.y -= int(round(p2->velocity.y));
			}

	
			
			p1->updateScreenPosition(p1->position.x - camera.x, p1->position.y - camera.y);
			p2->updateScreenPosition(p2->position.x - camera.x, p2->position.y - camera.y);
			DrawQuad(gRenderer, p1->screenPosition.x, p1->screenPosition.y,p1->radius*2,p1->radius*2);
			DrawCircle(gRenderer, p2->screenPosition.x, p2->screenPosition.y,p2->radius);
			DrawCircle(gRenderer, p1->screenPosition.x, p1->screenPosition.y, p2->radius);

			DrawDottedLine(gRenderer, p1->screenPosition.x, p1->screenPosition.y, treasureTile.x*50 + 25-camera.x, treasureTile.y*50 + 25 - camera.y);
			DrawDottedLine(gRenderer, p2->screenPosition.x, p2->screenPosition.y, treasureTile.x*50 + 25-camera.x, treasureTile.y*50 + 25 - camera.y);
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

void DrawCircle(SDL_Renderer* renderer, int x, int y, int radius)
{
	// it's there because the circle is drawn from the top left corner
	//x += radius;
	//y += radius;
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

void DrawQuad(SDL_Renderer* renderer, int x, int y, int width, int height)
{
	x -= width / 2;
	y -= height / 2;
	SDL_Color color;
	color.r = 255;
	color.g = 0;
	color.b = 0;
	color.a = 255;
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_Rect rect = { x,y,width,height };
	SDL_RenderFillRect(renderer, &rect);
}

void DrawDottedLine(SDL_Renderer* renderer, int x0, int y0, int x1, int y1) {
	int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
	int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
	int err = dx + dy, e2;
	int count = 0;
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	while (1) {
		if (count < 10) { SDL_RenderDrawPoint(renderer, x0, y0); }
		if (x0 == x1 && y0 == y1) break;
		e2 = 2 * err;
		if (e2 > dy) { err += dy; x0 += sx; }
		if (e2 < dx) { err += dx; y0 += sy; }
		count = (count + 1) % 20;
	}
}

Vector2f randomizeTreasure(std::vector<std::string>* map) {
	int numberOfColumns = LEVEL_WIDTH / TILE_SIZE;
	int numberOfRows = LEVEL_HEIGHT / TILE_SIZE;
	bool searching = true;
	for (int i = 0; i < numberOfColumns; i++) {
		for (int j = 0; j < numberOfRows; j++) {
			if (map->at(j).at(i) == 'w') {
				map->at(j).at(i) = 'x';
			}
		}
	}
	while (searching) {
		int x = rand() % numberOfColumns;
		int y = rand() % numberOfRows;
		if (map->at(y).at(x) == 'x') {
			map->at(y).at(x) = 'w';
			searching = false;
			return { float(x),float(y) };
		}
	}
}



