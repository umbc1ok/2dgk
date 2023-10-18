#include "Player.h"
#include <string>
#include <iostream>

Player::Player()
{
	Player::position.x = 0;
	Player::position.y = 0;
	Player::velocity.x = 0;
	Player::velocity.y = 0;
	Player::targetVelocity.x = 0;
	Player::targetVelocity.y = 0;

}

Player::~Player()
{
}

void Player::Move()
{
	float smooth = 0.94;
	// ten czas jest bez sensu, nie wiem jak to inaczej zrobic, ale to na pewno jest glupie
	velocity.x = targetVelocity.x * (1.0f - smooth) + velocity.x * smooth;
	velocity.y = targetVelocity.y * (1.0f - smooth) + velocity.y * smooth;
	position.x += int(round(velocity.x));
	position.y += int(round(velocity.y));
}
