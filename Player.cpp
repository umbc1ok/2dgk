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

void Player::Move(float time)
{
	float smooth = 0.84;
	// ten czas jest bez sensu, nie wiem jak to inaczej zrobic, ale to na pewno jest glupie
	velocity.x = float(targetVelocity.x) * (1 - smooth)  + velocity.x * smooth;
	velocity.y = float(targetVelocity.y) * (1 - smooth)  + velocity.y * smooth;
	std::cout << std::to_string(velocity.x) << std::endl;
	position.x += velocity.x;
	position.y += velocity.y;
}
