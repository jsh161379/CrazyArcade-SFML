#include "Player.h"

int dirRow(const std::string& dir) {
    if (dir == "down") return 0;
    if (dir == "up") return 1;
    if (dir == "left") return 2;
    return 3; // right
}

void updatePlayer(Player& p, float dt) {
    if (!p.moving) {
        int dx = 0, dy = 0;
        if (sf::Keyboard::isKeyPressed(p.keyUp)) { dy = -1; p.dir = "up"; }
        else if (sf::Keyboard::isKeyPressed(p.keyDown)) { dy = 1;  p.dir = "down"; }
        else if (sf::Keyboard::isKeyPressed(p.keyLeft)) { dx = -1; p.dir = "left"; }
        else if (sf::Keyboard::isKeyPressed(p.keyRight)) { dx = 1;  p.dir = "right"; }

        if (dx != 0 || dy != 0) {
            p.targetRow = p.row + dy;
            p.targetCol = p.col + dx;
            p.moving = true;
            p.progress = 0.f;
        }
    }
    else {
        p.progress += p.speed * dt;
        if (p.progress >= 1.f) {
            p.progress = 1.f;
            p.row = p.targetRow;
            p.col = p.targetCol;
            p.moving = false;
        }
    }

    if (p.moving) {
        p.animTimer += dt;
        if (p.animTimer >= p.animSpeed) {
            p.animTimer = 0.f;
            p.currentFrame = (p.currentFrame + 1) % FRAME_COUNT;
        }
    }
    else {
        p.currentFrame = 0;
        p.animTimer = 0.f;
    }
}

sf::Vector2f getRenderPos(const Player& p) {
    float px, py;
    if (p.moving) {
        px = (p.col + (p.targetCol - p.col) * p.progress) * TILE;
        py = (p.row + (p.targetRow - p.row) * p.progress) * TILE;
    }
    else {
        px = p.col * TILE;
        py = p.row * TILE;
    }
    return { px + TILE / 2.f, py + TILE / 2.f };
}