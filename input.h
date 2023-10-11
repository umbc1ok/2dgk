#pragma once
#include "Player.h"
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
			p1->y -= 10;
			break;

		case SDLK_DOWN:
			p1->y += 10;
			break;

		case SDLK_LEFT:
			p1->x -= 10;
			break;

		case SDLK_RIGHT:
			p1->x += 10;
			break;
		default:
			//gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
			break;
		}
	}
	else if (e->type == SDL_MOUSEMOTION) {
		SDL_GetMouseState(&p2->x, &p2->y);
	}
	return quit;
}