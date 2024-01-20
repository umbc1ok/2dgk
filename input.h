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
bool handleInput(SDL_Event* e, Player* p1, Player* p2, int maxSpeed){
	bool quit = false;
	if (e->type == SDL_QUIT)
	{
		quit = true;
	}

	if (e->type == SDL_KEYDOWN && e->key.repeat == 0)
	{
		switch (e->key.keysym.sym)
		{
		case SDLK_w:
			p1->handleJump();
			break;
		case SDLK_a:
			p1->targetVelocity.x = -maxSpeed;
			break;
		case SDLK_d:
			p1->targetVelocity.x = maxSpeed;
			break;
		case SDLK_1:
			p1->MAX_H += 10;
			if (p1->MAX_H <= 0) {
				p1->MAX_H = 1;
			}
			p1->calculateNewGravity();
			break;
		case SDLK_2:
			p1->MAX_H -= 10;
			p1->calculateNewGravity();
			break;
		case SDLK_3:
			p1->MAX_DISTANCE += 10;
			p1->calculateNewGravity();
			break;
		case SDLK_4:
			p1->MAX_DISTANCE -= 10;
			if (p1->MAX_DISTANCE <= 0) {
				p1->MAX_DISTANCE = 1;
			}
			p1->calculateNewGravity();
			break;
		case SDLK_ESCAPE:
			quit = true;
			break;
		}
	}
	else if (e->type == SDL_KEYUP && e->key.repeat == 0) {
		switch (e->key.keysym.sym)
		{
		case SDLK_w:
			p1->jumpPressed = false;
			break;
		case SDLK_a:
			p1->targetVelocity.x = 0;
			break;
		case SDLK_d:
			p1->targetVelocity.x = 0;
			break;
		}
	}
	/*else if (e->type == SDL_MOUSEMOTION) {
		SDL_GetMouseState(&p2->position.x, &p2->position.y);
		std::string log = "Mouse at:" + std::to_string(p2->position.x) + "," + std::to_string(p2->position.x);
		std::cout << log << std::endl;
	}*/
	return quit;
}

void handleKeyboardInput(const Uint8* currentKeyStates, Player* p1, Player* p2, int maxSpeed) {

	//PLAYER 1
	if (currentKeyStates[SDL_SCANCODE_UP])
	{
		//std::cout << "UP\n";
		p1->handleJump();
		
	}
	else {
		p1->jumpPressed = false;
	}

	if (currentKeyStates[SDL_SCANCODE_DOWN])
	{
		//std::cout << "DOWN\n";
		//p1->targetVelocity.y = maxSpeed;
	}

	if (currentKeyStates[SDL_SCANCODE_LEFT])
	{
		//std::cout << "LEFT\n";
		p1->targetVelocity.x = -maxSpeed;
	}

	if (currentKeyStates[SDL_SCANCODE_RIGHT])
	{
		//std::cout << "RIGHT\n";
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


	// PLAYER 2
	if (currentKeyStates[SDL_SCANCODE_W])
	{
		//std::cout << "UP\n";
		p2->targetVelocity.y = -maxSpeed;
	}

	if (currentKeyStates[SDL_SCANCODE_S])
	{
		//std::cout << "DOWN\n";
		p2->targetVelocity.y = maxSpeed;
	}

	if (currentKeyStates[SDL_SCANCODE_A])
	{
		//std::cout << "LEFT\n";
		p2->targetVelocity.x = -maxSpeed;
	}

	if (currentKeyStates[SDL_SCANCODE_D])
	{
		//std::cout << "RIGHT\n";
		p2->targetVelocity.x = maxSpeed;
	}
	if (currentKeyStates[SDL_SCANCODE_W] && currentKeyStates[SDL_SCANCODE_A])
	{
		p2->targetVelocity.y = -maxSpeed;
		p2->targetVelocity.x = -maxSpeed;
	}
	if (currentKeyStates[SDL_SCANCODE_W] && currentKeyStates[SDL_SCANCODE_D])
	{
		p2->targetVelocity.y = -maxSpeed;
		p2->targetVelocity.x = maxSpeed;
	}
	if (currentKeyStates[SDL_SCANCODE_S] && currentKeyStates[SDL_SCANCODE_A])
	{
		p2->targetVelocity.y = maxSpeed;
		p2->targetVelocity.x = -maxSpeed;
	}
	if (currentKeyStates[SDL_SCANCODE_S] && currentKeyStates[SDL_SCANCODE_D])
	{
		p2->targetVelocity.y = maxSpeed;
		p2->targetVelocity.x = maxSpeed;
	}


}
