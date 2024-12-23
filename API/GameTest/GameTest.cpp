///////////////////////////////////////////////////////////////////////////////
// Filename: GameTest.cpp
// Provides a demo of how to use the API
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
//------------------------------------------------------------------------
//Definitions
float objectX = 400.0f;
float objectY = 400.0f;
//------------------------------------------------------------------------
void Init()
{
    // Initial position of the object
    objectX = 400.0f;
    objectY = 400.0f;
}

void Update(float deltaTime)
{
    if (App::GetController().GetLeftThumbStickX() > 0.5f)
    {
        objectX += 1.0f;
    }
    if (App::GetController().GetLeftThumbStickX() < -0.5f)
    {
        objectX -= 1.0f;
    }
    if (App::GetController().GetLeftThumbStickY() > 0.5f)
    {
        objectY += 1.0f;
    }
    if (App::GetController().GetLeftThumbStickY() < -0.5f)
    {
        objectY -= 1.0f;
    }
}

void Render()
{
    // Draw a rectangle using lines
    float width = 50.0f;
    float height = 50.0f;

    // Top line
    App::DrawLine(objectX - width / 2, objectY + height / 2, objectX + width / 2, objectY + height / 2, 1.0f, 1.0f, 1.0f);
    // Bottom line
    App::DrawLine(objectX - width / 2, objectY - height / 2, objectX + width / 2, objectY - height / 2, 1.0f, 1.0f, 1.0f);
    // Left line
    App::DrawLine(objectX - width / 2, objectY - height / 2, objectX - width / 2, objectY + height / 2, 1.0f, 1.0f, 1.0f);
    // Right line
    App::DrawLine(objectX + width / 2, objectY - height / 2, objectX + width / 2, objectY + height / 2, 1.0f, 1.0f, 1.0f);

    // Print player number
    int num = Player::GetNum();
    std::string numStr = std::to_string(num);
    App::Print(400.0f, 500.0f, numStr.c_str());
}

void Shutdown()
{
    // No resources to clean up for line-drawn objects
}
