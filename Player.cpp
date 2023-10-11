#include "Player.h"

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
	position.x += velocity.x;
	position.y += velocity.y;
}
