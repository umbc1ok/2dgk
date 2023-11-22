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
}

Player::~Player()
{
}


void Player::updateScreenPosition(int x, int y) {
	screenPosition.x = x;
	screenPosition.y = y;
}



void Player::Move()
{
	float smooth = 0.94;
	// ten czas jest bez sensu, nie wiem jak to inaczej zrobic, ale to na pewno jest glupie
	velocity.x = targetVelocity.x * (1.0f - smooth) + velocity.x * smooth;
	velocity.y = targetVelocity.y * (1.0f - smooth) + velocity.y * smooth;


	// CHECKING IF PLAYER IS NOT ESCAPING THE SCREEN
	// THE 25's are assuming that players dimensions are 50x50 pixels
	if (screenPosition.x + 50 + int(round(velocity.x)) <= SCREEN_WIDTH && screenPosition.x + int(round(velocity.x)) >= 0) {
		position.x += int(round(velocity.x));
	}
	if (screenPosition.y + 50 + int(round(velocity.y)) <= SCREEN_HEIGHT && screenPosition.y + int(round(velocity.y)) >= 0) {
		position.y += int(round(velocity.y));
	}

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

