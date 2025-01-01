#include "stdafx.h"
#include "player.h"
#include "App/app.h"
#include "utility.h"

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

void Player::Draw()
{
    // Draw a rectangle using lines
    float width = 50.0f;
    float height = 50.0f;

	std::vector<std::vector<float>> points = {
		{x - width / 2, y + height / 2}, // Top left
		{x + width / 2, y + height / 2}, // Top right
		{x + width / 2, y - height / 2}, // Bottom right
		{x - width / 2, y - height / 2}  // Bottom left
	};
	Utility::DrawShape(points);

	//Draw red dot to show player's exact coordinate
	//points = {
	//	{x - 1, y + 1}, // Top left
	//	{x + 1, y + 1}, // Top right
	//	{x + 1, y - 1}, // Bottom right
	//	{x - 1, y - 1}  // Bottom left
	//};
	//Utility::DrawShape(points, 1.0f, 0.0f, 0.0f);
}