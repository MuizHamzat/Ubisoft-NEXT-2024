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
CSimpleSprite* sprite;
//------------------------------------------------------------------------
void Init()
{
	sprite = App::CreateSprite(".\\TestData\\ship.bmp", 1, 1);
	sprite->SetPosition(400.0f, 400.0f);
}

void Update(float deltaTime)
{
	sprite->Update(deltaTime);
	if (App::GetController().GetLeftThumbStickX() > 0.5f)
	{
		float x, y;
		sprite->GetPosition(x, y);
		sprite->SetPosition(x + 1.0f, y);
	}
	if (App::GetController().GetLeftThumbStickX() < -0.5f)
	{
		float x, y;
		sprite->GetPosition(x, y);
		sprite->SetPosition(x - 1.0f, y);
	}
	if (App::GetController().GetLeftThumbStickY() > 0.5f)
	{
		float x, y;
		sprite->GetPosition(x, y);
		sprite->SetPosition(x, y + 1.0f);
	}
	if (App::GetController().GetLeftThumbStickY() < -0.5f)
	{
		float x, y;
		sprite->GetPosition(x, y);
		sprite->SetPosition(x, y - 1.0f);
	}
}

void Render()
{
	sprite->Draw();
}

void Shutdown()
{
	delete sprite;
}