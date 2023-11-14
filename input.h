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
	else if (e->type == SDL_MOUSEMOTION) {
		SDL_GetMouseState(&p2->position.x, &p2->position.y);
		std::string log = "Mouse at:" + std::to_string(p2->position.x) + "," + std::to_string(p2->position.x);
		std::cout << log << std::endl;
	}
	return quit;
}

void handleKeyboardInput(const Uint8* currentKeyStates, Player* p1, int maxSpeed) {
	if (currentKeyStates[SDL_SCANCODE_UP])
	{
		std::cout << "UP\n";
		p1->targetVelocity.y = -maxSpeed;
	}

	if (currentKeyStates[SDL_SCANCODE_DOWN])
	{
		std::cout << "DOWN\n";
		p1->targetVelocity.y = maxSpeed;
	}

	if (currentKeyStates[SDL_SCANCODE_LEFT])
	{
		std::cout << "LEFT\n";
		p1->targetVelocity.x = -maxSpeed;
	}

	if (currentKeyStates[SDL_SCANCODE_RIGHT])
	{
		std::cout << "RIGHT\n";
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
}
