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
SDL_Texture* floorTexture = NULL;
SDL_Texture* groundTexture = NULL;
SDL_Texture* skyTexture = NULL;
SDL_Texture* cloudTexture = NULL;
SDL_Texture* fernTexture = NULL;
SDL_Texture* starredTexture = NULL;
SDL_Texture* playerTexture = NULL;
SDL_Texture* frames[8];




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

	int currentAnimFrame = 0;
	Uint64 animTime = 0;

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




	float layerSpeed[3] = { 0.1,0.4,1.0 };


	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		Uint64 PREVIOUS = 0;
		Uint64 CURRENT = 0;
		Uint64 deltaTime = 0;
		Uint64 desiredFrameTime = 17; // 1/60 of a second


		char arr[] = "tiles/floor.png";
		loadMedia(arr, &floorTexture, gRenderer);
		char arr2[] = "tiles/ground.png";
		loadMedia(arr2, &groundTexture, gRenderer);
		char arr3[] = "tiles/background.png";
		loadMedia(arr3, &skyTexture, gRenderer);
		char arr4[] = "tiles/cloud.png";
		loadMedia(arr4, &cloudTexture, gRenderer);
		char arr5[] = "tiles/fern.png";
		loadMedia(arr5, &fernTexture, gRenderer);
		char arr6[] = "tiles/starred.png";
		loadMedia(arr6, &starredTexture, gRenderer);
		char arr7[] = "tiles/player.png";
		loadMedia(arr7, &playerTexture, gRenderer);

		char animFrameName1[] = "anim/1.png";
		loadMedia(animFrameName1, &frames[0], gRenderer);
		char animFrameName2[] = "anim/2.png";
		loadMedia(animFrameName2, &frames[1], gRenderer);
		char animFrameName3[] = "anim/3.png";
		loadMedia(animFrameName3, &frames[2], gRenderer);
		char animFrameName4[] = "anim/4.png";
		loadMedia(animFrameName4, &frames[3], gRenderer);
		char animFrameName5[] = "anim/5.png";
		loadMedia(animFrameName5, &frames[4], gRenderer);
		char animFrameName6[] = "anim/6.png";
		loadMedia(animFrameName6, &frames[5], gRenderer);
		char animFrameName7[] = "anim/7.png";
		loadMedia(animFrameName7, &frames[6], gRenderer);
		char animFrameName8[] = "anim/8.png";
		loadMedia(animFrameName8, &frames[7], gRenderer);


		std::vector<std::string>* map = loadMapFromFile("map2.txt");
		std::vector<std::string>* map2 = loadMapFromFile("map3.txt");
		std::vector<std::string>* map3 = loadMapFromFile("map4.txt");

		//Vector2f treasureTile = randomizeTreasure(map);




		int targetX = 0;
		int targetY = 0;
		float cameraSmooth = 0.93f;
		bool collisions = true;
		bool separation = true;
		int currentMap = 0;
		CURRENT = 0;
		while (!quit) {
			if (currentMap != scoreboard->checkCurrentMap()) {
				currentMap = scoreboard->checkCurrentMap();
				p1->position = { rand() % 750 + 50,rand() % 500 + 50 };
				p2->position = { rand() % 750 + 50,rand() % 500 + 50 };
				std::cout << currentMap << std::endl;
				if (currentMap == 0) {
					map = loadMapFromFile("map2.txt");
					int positionSetIndex = rand() % (spawnPoints1.size() / 2);
					p1->position = spawnPoints1.at(positionSetIndex * 2);
					p2->position = spawnPoints1.at(positionSetIndex * 2 + 1);
				}
				else if (currentMap == 1) {
					map = loadMapFromFile("map3.txt");
					int positionSetIndex = rand() % (spawnPoints2.size() / 2);
					p1->position = spawnPoints2.at(positionSetIndex * 2);
					p2->position = spawnPoints2.at(positionSetIndex * 2 + 1);
				}
				else if (currentMap == 2) {
					map = loadMapFromFile("map4.txt");
					int positionSetIndex = rand() % (spawnPoints3.size() / 2);
					p1->position = spawnPoints3.at(positionSetIndex * 2);
					p2->position = spawnPoints3.at(positionSetIndex * 2 + 1);
				}
				//treasureTile = randomizeTreasure(map);
			}
			///////////////////////
			// FRAMETIME COUNTER //
			///////////////////////

			PREVIOUS = CURRENT;
			CURRENT = SDL_GetPerformanceCounter();
			if (PREVIOUS == 0) {
				PREVIOUS = CURRENT;
			}
			deltaTime = (CURRENT - PREVIOUS) * 1000 / static_cast<float>(SDL_GetPerformanceFrequency());

			/////////////////////
			// CAMERA MOVEMENT //
			/////////////////////

			// double P1 is on purpose, i didnt want to change the script when I have 1 player
			moveCamera(&camera, p1, p1, boundingBox, &targetX, &targetY);

			/////////////////////////////////////
			// Handle mouse and keyboard input //
			/////////////////////////////////////

			// KEYBOARD (PLAYER 1)
			const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

			

			int maxSpeed = 3;
			//handleKeyboardInput(currentKeyStates, p1,p2,maxSpeed);


			bool pushed = false;
			bool walking = false;
			while (SDL_PollEvent(&e) != 0) {
				walking = handleInput(&e, p1, p2, maxSpeed, layerSpeed);
			}
			if (walking) {
				animTime += deltaTime;
				if (animTime > currentAnimFrame * 100) {
					animTime = 0;
					currentAnimFrame = (currentAnimFrame + 1) % 8;
					playerTexture = frames[currentAnimFrame];
					currentAnimFrame++;
				}

			}
			else {
				animTime = 0;
			}

			/////////////////
			// MAP DRAWING //
			/////////////////
			// BACKGROUND
			int numberOfColumns = LEVEL_WIDTH / TILE_SIZE;
			int numberOfRows = LEVEL_HEIGHT / TILE_SIZE;
			for (int i = 0; i < numberOfRows; i++) {
				std::string line = map3->at(i);
				for (int j = 0; j < numberOfColumns; j++) {
					drawElement(j * TILE_SIZE - camera.x * layerSpeed[0], i * TILE_SIZE - camera.y, line.at(j), gRenderer, floorTexture, groundTexture, skyTexture, cloudTexture, fernTexture, starredTexture);
				}
			}


			// SECOND LAYER
			for (int i = 0; i < numberOfRows; i++) {
				std::string line = map2->at(i);
				for (int j = 0; j < numberOfColumns; j++) {
					drawElement(j * TILE_SIZE - camera.x * layerSpeed[1], i* TILE_SIZE - camera.y, line.at(j), gRenderer, floorTexture, groundTexture, skyTexture, cloudTexture, fernTexture, starredTexture);
				}
			}

			// FRONT LAYER
			for (int i = 0; i < numberOfRows; i++) {
				std::string line = map->at(i);
				for (int j = 0; j < numberOfColumns; j++) {
					drawElement(j * TILE_SIZE - camera.x * layerSpeed[2], i * TILE_SIZE - camera.y, line.at(j), gRenderer, floorTexture, groundTexture, skyTexture, cloudTexture, fernTexture, starredTexture);
				}
			}





			p1->jump(CURRENT, PREVIOUS, deltaTime);
			if (collideWithLabirynthWalls(p1, *map, true, scoreboard, false)) {
				p1->position.y -= int((p1->tempVelocity.y));
				p1->velocity.y = 0;
			}


			p1->MoveX();
			if (collideWithLabirynthWalls(p1, *map, true, scoreboard, true)) {
				//p1->position.x -= int(round(p1->velocity.x));
			}

			//std::cout << p1->velocity.x << std::endl;





			p1->updateScreenPosition(p1->position.x - camera.x, p1->position.y - camera.y);
			//DrawQuad(gRenderer, p1->screenPosition.x, p1->screenPosition.y,p1->radius*2, p1->radius*2);
			//DrawCircle(gRenderer, p1->screenPosition.x, p1->screenPosition.y, p1->radius);

			
			drawPlayer(gRenderer, playerTexture, p1->screenPosition.x - 32, p1->screenPosition.y -48);


			//DrawDottedLine(gRenderer, p1->screenPosition.x, p1->screenPosition.y, treasureTile.x*50 + 25-camera.x, treasureTile.y*50 + 25 - camera.y);
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
	//SDL_DestroyTexture(woodTexture);
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



