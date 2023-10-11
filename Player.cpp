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

void Player::Move(float time)
{
	float smooth = 0.95;
	velocity.x = float(targetVelocity.x) * (1 - smooth) * time + velocity.x * smooth;
	velocity.y = float(targetVelocity.y) * (1 - smooth) * time + velocity.y * smooth;
	position.x += velocity.x;
	position.y += velocity.y;
}
