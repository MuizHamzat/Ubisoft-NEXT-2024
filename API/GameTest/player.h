#pragma once //Does the exact same thing as #ifndef PLAYER_H and #define PLAYER_H. It's just more consice

class Player
{
public:
    Player(float xpos, float ypos);

    std::vector<float> GetPosition() const;
    void SetPosition(float xpos, float ypos);
    void Draw();

    float x, y;

private:
    
};