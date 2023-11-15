#pragma once

#include "dataStructures.h"
class Player {
public:
	Player();
	~Player();
	Vector2i position;
	Vector2i screenPosition;
	Vector2f velocity;
	Vector2f targetVelocity;
	void updateScreenPosition(int x, int y);
	void Move();
	void fixPosition();
};

