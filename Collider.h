#pragma once
#include "dataStructures.h"

class Collider {
public:
	Collider(Vector2i _hitBox);
	~Collider();

	Vector2i hitBox;



	void checkCollision(Collider other);
	


};