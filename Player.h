#pragma once

#include "dataStructures.h"
class Player {
public:
	Player();
	~Player();
	void checkCollision(Player other);
	void wallCollision();
	Vector2i position;
	Vector2i dimensions;
	Vector2i screenPosition;
	Vector2f velocity;
	Vector2f targetVelocity;
	float radius;



	void updateScreenPosition(int x, int y);
	void Move();
	void fixPosition();
	void separate(Player other);
};

