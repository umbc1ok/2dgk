#pragma once
#include "Player.h"
#include <string>
enum KeyPressSurfaces
{
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL
};

bool handleInput(SDL_Event* e, Player* p1, Player* p2 ){
	bool quit = false;
	if (e->type == SDL_QUIT)
	{
		quit = true;
	}
	else if (e->type == SDL_KEYDOWN)
	{
		//Select surfaces based on key press
		switch (e->key.keysym.sym)
		{
		case SDLK_UP:
			printf("UP\n");
			p1->y -= p1->velocity;
			break;

		case SDLK_DOWN:
			printf("DOWN\n");
			p1->y += p1->velocity;
			break;

		case SDLK_LEFT:
			printf("LEFT\n");
			p1->x -= p1->velocity;
			break;

		case SDLK_RIGHT:
			printf("LEFT\n");
			p1->x += p1->velocity;
			break;
		default:
			//gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
			break;
		}
	}
	else if (e->type == SDL_MOUSEMOTION) {
		SDL_GetMouseState(&p2->x, &p2->y);
		std::string log = "Mouse at:" + std::to_string(p2->x) + "," + std::to_string(p2->x);
		std::cout << log << std::endl;
	}
	return quit;
}