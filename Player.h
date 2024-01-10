#pragma once

#include "dataStructures.h"
#include <SDL_stdinc.h>
#include "definitions.h"
class Player {
public:
	Player();
	~Player();
	void checkCollision(Player other);
	bool wallCollision();
	Vector2i position;
	Vector2i dimensions;
	Vector2i screenPosition;
	Vector2f velocity;
	Vector2f tempVelocity;
	Vector2f targetVelocity;
	float radius;

	float gravity;
	bool jumpPressed;
	float jumpTime;
	int jumpCount;

	float MAX_H = TILE_SIZE * 2;
	float MAX_DISTANCE = TILE_SIZE * 6;
	float V_0;

	static const int VEL = 2;


	void updateScreenPosition(int x, int y);
	void MoveX();
	void MoveY();
	void fixPosition();
	void separate(Player other);
	void handleJump();
	void jump(Uint64 currentTime, Uint64 prevTime, Uint64 deltaTime);
	void calculateNewGravity();
};

