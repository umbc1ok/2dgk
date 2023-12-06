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


// mouse input only
bool handleInput(SDL_Event* e, Player* p1, Player* p2 ){
	bool quit = false;
	if (e->type == SDL_QUIT)
	{
		quit = true;
	}
	/*else if (e->type == SDL_MOUSEMOTION) {
		SDL_GetMouseState(&p2->position.x, &p2->position.y);
		std::string log = "Mouse at:" + std::to_string(p2->position.x) + "," + std::to_string(p2->position.x);
		std::cout << log << std::endl;
	}*/
	return quit;
}

void handleKeyboardInput(const Uint8* currentKeyStates, bool& collisions, bool& separation) {

	//PLAYER 1
	if (currentKeyStates[SDL_SCANCODE_C])
	{
		collisions = 1;
		std::cout << "Collisions are now " << collisions << std::endl;
	}

	if (currentKeyStates[SDL_SCANCODE_V])
	{
		collisions = 0;
		std::cout << "Collisions are now " << collisions << std::endl;
	}

	if (currentKeyStates[SDL_SCANCODE_S])
	{
		separation = 1;
		std::cout << "Separations is now " << separation << std::endl;
	}
	if (currentKeyStates[SDL_SCANCODE_D])
	{
		separation = 0;
		std::cout << "Separations is now " << separation << std::endl;
	}
}
