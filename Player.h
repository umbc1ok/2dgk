#pragma once

#include "dataStructures.h"
class Player {
public:
	Player();
	~Player();
	Vector2i position;
	Vector2f velocity;
	Vector2f targetVelocity;
	void Move();
};

