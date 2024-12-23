#include "stdafx.h"
#include "player.h"
#include "app/app.h"

Player::Player(float xpos, float ypos) : x(xpos), y(ypos){}

std::vector<float> Player::GetPosition() const
{
	return std::vector<float>{x, y};
}

void Player::SetPosition(float xpos, float ypos)
{
    x = xpos;
    y = ypos;
}
