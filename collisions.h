#pragma once
#include "dataStructures.h"
#include "definitions.h"
#include "Player.h"
#include <vector>
#include <string>
#include <iostream>
#include "scoreboard.h"

//currently the problem si that when I cross onto another tile, the collision is detected even if it's a corridor tile


float clamp(float value, float min, float max);
bool checkIfCircleAndRectOverlap(Player* player, int tileXIndex, int tileYIndex, bool collide);
void collideWithLabirynthWalls(Player* player, std::vector<std::string> map, bool circle, Scoreboard* scrb);
void separate(Player* player, float tileX, float tileY, Vector2f closestPoint);
bool checkIfRectAndRectOverlap(Player* player, int tileXIndex, int tileYIndex, bool collide);

//function for a circle colliding with a rectangluar wall hitbox
void collideWithLabirynthWalls(Player* player, std::vector<std::string> map, bool circle, Scoreboard* scrb) {
	//find the tile that the player's center is in
	int tileX = player->position.x / TILE_SIZE;
	int tileY = player->position.y / TILE_SIZE;

	// go over them and check if they collide with the player
	for (int i = -1; i < 2; i++) {
		for (int j = -1; j < 2; j++) {
			//honestly copilot told me this condition, but idk how the hell it works, but it does
			if (tileY + i >= 0 && tileY + i < map.size() && tileX + j >= 0 && tileX + j < map.at(0).size()) {
				if (map[tileY + i][tileX + j] == '=') {
					if (circle)
					{
						checkIfCircleAndRectOverlap(player, tileX + j, tileY + i, true);
					}
					else
					{
						checkIfRectAndRectOverlap(player, tileX + j, tileY + i, true);
					}
				}
				if (map[tileY + i][tileX + j] == 'w') {
					if (circle)
					{
						if (checkIfCircleAndRectOverlap(player, tileX + j, tileY + i, false)) {
							scrb->win(1);
							return;
						}
					}
					else
					{
						if (checkIfRectAndRectOverlap(player, tileX + j, tileY + i, false)) {
							scrb->win(2);
							return;
						}

					}
				}
			}
		}
	}
}
	// if they do, move the player by the separation vector

bool checkIfCircleAndRectOverlap(Player* player, int tileXIndex, int tileYIndex, bool collide) {
	Vector2f closestPoint;
	closestPoint.x = clamp(player->position.x, tileXIndex * TILE_SIZE, tileXIndex * TILE_SIZE + TILE_SIZE);
	closestPoint.y = clamp(player->position.y, tileYIndex * TILE_SIZE, tileYIndex * TILE_SIZE + TILE_SIZE);

	float distance;
	distance = sqrt(pow(player->position.x - closestPoint.x, 2) + pow(player->position.y - closestPoint.y, 2));
	
	if (distance < player->radius) {
		if(collide) separate(player, tileXIndex, tileYIndex, closestPoint);
		return true;
	}
	else {
		return false;
	}
}

bool checkIfRectAndRectOverlap(Player* player, int tileXIndex, int tileYIndex, bool collide) {
	float playerLeft = player->position.x - player->radius;
	float playerRight = player->position.x + player->radius;
	float playerTop = player->position.y - player->radius;
	float playerBottom = player->position.y + player->radius;



	//overlaps 
	float left = playerRight - tileXIndex * TILE_SIZE;
	float right = tileXIndex * TILE_SIZE + TILE_SIZE - playerLeft;
	float top = playerBottom - tileYIndex * TILE_SIZE;
	float bottom = tileYIndex * TILE_SIZE + TILE_SIZE - playerTop;

	if (left <= 0 || right <= 0 || top <= 0 || bottom <= 0) {
		return false;
	}
	else {
		if(collide){
			Vector2f separation;
			separation.x = left < right ? -left : right;
			separation.y = top < bottom ? -top : bottom;

			if (separation.x < separation.y) {
				separation.y = 0;
			}
			else if (separation.x > separation.y) {
				separation.x = 0;
			}
			if (separation.x != 0) {
				player->velocity.x = 0;
				player->targetVelocity.x = 0;
			}
			if (separation.y != 0) {
				player->velocity.y = 0;
				player->targetVelocity.y = 0;
			}
			player->position.x += separation.x;
			player->position.y += separation.y;
		}

		return true;
	}
}


float clamp(float value, float min, float max) {
	if (value <= min) {
		return min;
	}
	else if (value >= max) {
		return max;
	}
	else {
		return value;
	}
}

void separate(Player* player, float tileX, float tileY, Vector2f closestPoint) {
	Vector2f separation;
	if (player->position.x == closestPoint.x && player->position.y == closestPoint.y) {
		float left = player->position.x - tileX * TILE_SIZE + player->radius;
		float right = tileX* TILE_SIZE + TILE_SIZE - player->position.x + player->radius;
		float top = player->position.y - tileY * TILE_SIZE + player->radius;
		float bottom = tileY * TILE_SIZE + TILE_SIZE - player->position.y + player->radius;
		separation.x = left < right ? -left : right;
		separation.y = top < bottom ? -top : bottom;
	}
	else {
		separation.x = player->position.x - closestPoint.x;
		separation.y = player->position.y - closestPoint.y;

		float distance = sqrt(separation.x * separation.x + separation.y * separation.y);
		float factor = (player->radius - distance) / distance;

		separation.x *= factor;
		separation.y *= factor;
	}
	player->position.x += separation.x;
	player->position.y += separation.y;
}