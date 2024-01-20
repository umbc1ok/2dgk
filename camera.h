#pragma once
#include <SDL.h>
#include "definitions.h"

void keepCameraInBounds(SDL_Rect* camera);
void moveCamera(SDL_Rect* camera, Player* p1, Player* p2, Vector2i boundingBox, int* targetX, int* targetY);


void moveCamera(SDL_Rect* camera, Player* p1, Player* p2, Vector2i boundingBox, int* targetX, int* targetY) {
	
	if (abs(camera->x + SCREEN_WIDTH / 2 - p1->position.x - 25) >= boundingBox.x
		|| abs(camera->x + SCREEN_WIDTH / 2 - p2->position.x) >= boundingBox.x) { // so far it's a circle so we dont have to subtract 25
		*targetX = (p1->position.x + 25 + p2->position.x) / 2.0f - SCREEN_WIDTH / 2;				     // same here with the +25
	}
	if (abs(camera->y + SCREEN_HEIGHT / 2 - p1->position.y - 25) >= boundingBox.y
		|| abs(camera->y + SCREEN_HEIGHT / 2 - p2->position.y) >= boundingBox.x) { // so far it's a circle so we dont have to subtract 25
		*targetY = (p1->position.y + 25 + p2->position.y) / 2.0f - SCREEN_HEIGHT / 2;				     // same here with the +25
	}
	camera->x = *targetX * (1.0f - CAMERA_SMOOTH) + camera->x * CAMERA_SMOOTH;
	camera->y = *targetY * (1.0f - CAMERA_SMOOTH) + camera->y * CAMERA_SMOOTH;
	/*
	camera->x = ((p1->position.x - SCREEN_WIDTH / 2 + p1->radius) + (p2->position.x - SCREEN_WIDTH / 2 + p2->radius)) / 2.0f;
	camera->y = ((p1->position.y - SCREEN_HEIGHT / 2 + p1->radius) + (p2->position.y - SCREEN_HEIGHT / 2 + p2->radius)) / 2.0f;
	*/
	keepCameraInBounds(camera);
}
// 1 player version
/*
void moveCamera(SDL_Rect* camera, Player* p1, Player* p2, Vector2i boundingBox, int* targetX) {
	if (abs(camera->x + SCREEN_WIDTH / 2 - p1->position.x - 25) >= boundingBox.x) { // boundingBox.x is the width of the bounding box
		// the 25 is half of players width
		*targetX = p1->position.x - SCREEN_WIDTH / 2 + 25;					     // same here with the +25
	}

	camera->x = *targetX * (1.0f - CAMERA_SMOOTH) + camera->x * CAMERA_SMOOTH;
	camera->y = p1->position.y - SCREEN_HEIGHT / 2 + 25;

	keepCameraInBounds(camera);
}*/


void keepCameraInBounds(SDL_Rect* camera) {
	if (camera->x < 0)
	{
		camera->x = 0;
	}
	if (camera->y < 0)
	{
		camera->y = 0;
	}
	if (camera->x > LEVEL_WIDTH - camera->w)
	{
		camera->x = LEVEL_WIDTH - camera->w;
	}
	if (camera->y > LEVEL_HEIGHT - camera->h)
	{
		camera->y = LEVEL_HEIGHT - camera->h;
	}
}