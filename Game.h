#pragma once

#include <SFML/Graphics.hpp>
#include "TileMap.h"
#include "Player.h"

enum class GameState
{
    Start,
    Playing,
    GameOver
};

class Game
{
private:
    // 게임 화면 크기
    static constexpr unsigned int WINDOW_WIDTH = 800;
    static constexpr unsigned int WINDOW_HEIGHT = 600;

    // 맵 정보
    static constexpr int MAP_COLS = 15;
    static constexpr int MAP_ROWS = 13;
    static constexpr float TILE_SIZE = 40.f;

    sf::RenderWindow window;
    GameState state;
    sf::Clock clock;

    sf::Font font;

    // Start 화면
    sf::Texture startBackgroundTexture;
    sf::Sprite startBackground;

    sf::RectangleShape startButtonShadow;
    sf::RectangleShape startButton;
    sf::Text startText;

    // Playing 화면
    TileMap tileMap;
    Player player1;
    Player player2;
    sf::RectangleShape uiArea;

    // 승자 정보
    int winner;

    // GameOver 화면 배경
    sf::Texture resultBackgroundTexture;
    sf::Sprite resultBackground;

    // 결과 화면 반투명 패널
    sf::RectangleShape resultPanel;

    sf::Text winnerText;

    sf::RectangleShape restartButton;
    sf::Text restartText;

    sf::RectangleShape exitButton;
    sf::Text exitText;

    void processEvents();
    void update(float deltaTime);
    void render();
    void resetGame();
    void setWinner(int playerNumber);

    void centerTextOnButton(
        sf::Text& text,
        const sf::RectangleShape& button);

    void centerTextX(
        sf::Text& text,
        float y);

public:
    Game();
    void run();
};