#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class TileMap;


// ============================================================
// Sprite Sheet 설정
// ============================================================

constexpr int FRAME_W = 313;
constexpr int FRAME_H = 313;
constexpr int FRAME_COUNT = 4;


// ============================================================
// Player
// ============================================================

struct Player
{
    // --------------------------------------------------------
    // 타일 위치
    // row = y
    // col = x
    // --------------------------------------------------------

    int row = 1;
    int col = 1;

    int targetRow = 1;
    int targetCol = 1;


    // --------------------------------------------------------
    // 이동
    // --------------------------------------------------------

    bool moving = false;

    // 현재 타일 -> 목표 타일 진행도
    // 0.0 ~ 1.0
    float progress = 0.f;

    // 초당 이동 타일 수
    float speed = 4.f;


    // --------------------------------------------------------
    // 상태
    // --------------------------------------------------------

    bool alive = true;
    bool trapped = false;


    // --------------------------------------------------------
    // 능력치
    // --------------------------------------------------------

    int bombCapacity = 1;
    int blastRange = 1;
    int needles = 0;


    // --------------------------------------------------------
    // 애니메이션
    // --------------------------------------------------------

    std::string dir = "down";

    int currentFrame = 0;

    float animTimer = 0.f;
    float animSpeed = 0.12f;


    // --------------------------------------------------------
    // 조작키
    // --------------------------------------------------------

    sf::Keyboard::Scancode keyUp{};
    sf::Keyboard::Scancode keyDown{};
    sf::Keyboard::Scancode keyLeft{};
    sf::Keyboard::Scancode keyRight{};


    // --------------------------------------------------------
    // 캐릭터 이미지
    // --------------------------------------------------------

    sf::Texture texture;

    bool textureLoaded = false;

    float drawSize = 36.f;


    // --------------------------------------------------------
    // Texture 로드
    // --------------------------------------------------------

    bool loadTexture(const std::string& path);


    // --------------------------------------------------------
    // 화면 출력
    // --------------------------------------------------------

    void draw(sf::RenderWindow& window) const;
};


// ============================================================
// 함수
// ============================================================

int dirRow(const std::string& dir);


void updatePlayer(
    Player& p,
    const TileMap& map,
    float dt
);


sf::Vector2f getRenderPos(
    const Player& p
);


sf::Vector2i getTilePosition(
    const Player& p
);


void killPlayer(
    Player& p
);


bool isAlive(
    const Player& p
);