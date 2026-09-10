#pragma once
#include <SFML/Graphics.hpp>
#include <string>

const float TILE = 44.f;
const int FRAME_W = 313;
const int FRAME_H = 313;
const int FRAME_COUNT = 4;   // 한 방향당 프레임 수

struct Player {
    int row, col;
    int targetRow, targetCol;
    bool moving = false;
    float progress = 0.f;
    float speed = 4.f;

    std::string dir = "down";
    int currentFrame = 0;
    float animTimer = 0.f;
    float animSpeed = 0.12f;

    sf::Keyboard::Scancode keyUp, keyDown, keyLeft, keyRight;
};

int dirRow(const std::string& dir);
void updatePlayer(Player& p, float dt);
sf::Vector2f getRenderPos(const Player& p);
