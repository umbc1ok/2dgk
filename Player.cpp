#include "Player.h"
#include <string>
#include <iostream>
#include "definitions.h"

Player::Player()
{
	Player::position.x = 0;
	Player::position.y = 0;
	Player::velocity.x = 0;
	Player::velocity.y = 0;
	Player::targetVelocity.x = 0;
	Player::targetVelocity.y = 0;
	Player::screenPosition.x = 0;
	Player::screenPosition.y = 0;
	Player::radius = 30.0f;

}

Player::~Player()
{
}



void Player::checkCollision(Player other) {
	float xDistance = abs((this->position.x + this->radius / 2.0f) - (other.position.x + other.radius / 2.0f));
	float yDistance = abs((this->position.y + this->radius / 2.0f) - (other.position.y + other.radius / 2.0f));
	
	float distance = sqrt(xDistance * xDistance + yDistance * yDistance);

	if (distance < radius + other.radius) {
		Vector2f separation;
		separation.x = this->position.x - other.position.x;
		separation.y = this->position.y - other.position.y;

		float factor = (this->radius + other.radius - distance) / distance;
		separation.x *= factor;
		separation.y *= factor;
		
		float sepLength = sqrt(separation.x * separation.x + separation.y * separation.y);
		separation.x /= sepLength;
		separation.y /= sepLength;
		Vector2f newVelocity;
		newVelocity.x = velocity.x - 2 * (separation.x * velocity.x + separation.y * velocity.y) * separation.x;
		newVelocity.y = velocity.y - (2 * (separation.x * velocity.x + separation.y * velocity.y)) * separation.y;
		velocity.x = newVelocity.x;
		velocity.y = newVelocity.y;
	}
}

void Player::wallCollision() {
	if (position.x + radius * 2> SCREEN_WIDTH || position.x - radius < 0) {
		velocity.x *= -1;
	}
	if (position.y + radius > SCREEN_HEIGHT || position.y - radius < 0) {
		velocity.y *= -1;
	}

}


void Player::updateScreenPosition(int x, int y) {
	screenPosition.x = x;
	screenPosition.y = y;
}



void Player::Move()
{
	/*
	float smooth = 0.94;
	// ten czas jest bez sensu, nie wiem jak to inaczej zrobic, ale to na pewno jest glupie
	velocity.x = targetVelocity.x * (1.0f - smooth) + velocity.x * smooth;
	velocity.y = targetVelocity.y * (1.0f - smooth) + velocity.y * smooth;

	*/
	// CHECKING IF PLAYER IS NOT ESCAPING THE SCREEN
	// THE 25's are assuming that players dimensions are 50x50 pixels
	

	wallCollision();


	//if (screenPosition.x + 50 + int(round(velocity.x)) <= SCREEN_WIDTH && screenPosition.x + int(round(velocity.x)) >= 0) {
		position.x += int(round(velocity.x));
	//}
	//if (screenPosition.y + 50 + int(round(velocity.y)) <= SCREEN_HEIGHT && screenPosition.y + int(round(velocity.y)) >= 0) {
		position.y += int(round(velocity.y));
	//}

	

}


// for some reason when one person is going left, and the other is going right
// the right one gets obliterated
// Problem might not be here
// But before this chunk of code was added, the player would just get stuck on the right border
// until the other player moved towards the middle of the screen to move the camera window
void Player::fixPosition()
{
	if (screenPosition.x < 0) {
		position.x -= screenPosition.x; //move the player by how much he was out of the box
		screenPosition.x = 0;
		targetVelocity.x = 0;
	}
	if (screenPosition.y < 0) {
		position.y -= screenPosition.y; //move the player by how much he was out of the box
		screenPosition.y = 0;
		targetVelocity.y = 0;
	}
	if (screenPosition.x + 50 > SCREEN_WIDTH) {
		position.x -= screenPosition.x - SCREEN_WIDTH; //move the player by how much he was out of the box
		screenPosition.x = SCREEN_WIDTH - 50;
		targetVelocity.x = 0;
	}
	if (screenPosition.y + 50 > SCREEN_HEIGHT) {
		position.y -= screenPosition.y - SCREEN_HEIGHT; //move the player by how much he was out of the box
		screenPosition.y = SCREEN_HEIGHT - 50;
		targetVelocity.y = 0;
	}
}

void Player::separate(Player other)
{

	float xDistance = abs((this->position.x + this->radius / 2.0f) - (other.position.x + other.radius / 2.0f));
	float yDistance = abs((this->position.y + this->radius / 2) - (other.position.y + other.radius / 2));

	float distance = sqrt(xDistance * xDistance + yDistance * yDistance);

	if (distance < radius + other.radius) {
		float overlap = 0.5f * ( distance - radius - other.radius);
		this->position.x -= overlap * (this->position.x - other.position.x) / distance;
		this->position.y -= overlap * (this->position.y - other.position.y) / distance;
	}

}

