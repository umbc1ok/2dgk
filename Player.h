#pragma once

#include "dataStructures.h"
class Player {
public:
	Player();
	~Player();
	Vector2 position;
	Vector2 velocity;
	Vector2 targetVelocity;
	void Move();
};

