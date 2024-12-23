// GameTest.cpp
///////////////////////////////////////////////////////////////////////////////
// Filename: GameTest.cpp
// Provides a demo of how to use the API
///////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------
#include "stdafx.h"
//------------------------------------------------------------------------
#include <windows.h> 
#include <math.h>  
//------------------------------------------------------------------------
#include "app\app.h"
#include "player.h"
#include "utility.h"
//------------------------------------------------------------------------
//Definitions
Player player(400.0f, 400.0f);
//------------------------------------------------------------------------
void Init()
{
    // Initial position of the object
    player.SetPosition(400.0f, 400.0f);
}

void Update(float deltaTime)
{
    if (App::GetController().GetLeftThumbStickX() > 0.5f)
    {
        player.x += 1;
    }
    if (App::GetController().GetLeftThumbStickX() < -0.5f)
    {
        player.x -= 1;
    }
    if (App::GetController().GetLeftThumbStickY() > 0.5f)
    {
        player.y += 1;
    }
    if (App::GetController().GetLeftThumbStickY() < -0.5f)
    {
        player.y -= 1;
    }
}

void Render()
{
    // Draw a rectangle using lines
    float width = 50.0f;
    float height = 50.0f;

    // Top line
    App::DrawLine(player.x - width / 2, player.y + height / 2, player.x + width / 2, player.y + height / 2);
    // Bottom line
    App::DrawLine(player.x - width / 2, player.y - height / 2, player.x + width / 2, player.y - height / 2);
    // Left line
    App::DrawLine(player.x - width / 2, player.y - height / 2, player.x - width / 2, player.y + height / 2);
    // Right line
    App::DrawLine(player.x + width / 2, player.y - height / 2, player.x + width / 2, player.y + height / 2);

    // Print player number
	std::vector<float> pos = player.GetPosition();
	std::string posStr = Utility::VectorToString(pos);
    App::Print(400.0f, 500.0f, posStr.c_str());
}

void Shutdown()
{
    // No resources to clean up for line-drawn objects
}

