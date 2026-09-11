#pragma once

#include <SFML/Graphics.hpp>
#include <array>
#include <memory>
#include "Item.h"
#include "Player.h"
#include "TileMap.h"
#include "UI.h"

class WaterBalloonSystem;

enum class GameState
{
    Start,
    CharacterSelect,
    Playing,
    GameOver
};

class Game
{
private:
    // ���� ȭ�� ũ��
    static constexpr unsigned int WINDOW_WIDTH = 800;
    static constexpr unsigned int WINDOW_HEIGHT = 600;

    // �� ����
    static constexpr int MAP_COLS = 15;
    static constexpr int MAP_ROWS = 13;
    static constexpr float TILE_SIZE = 40.f;

    sf::RenderWindow window;
    GameState state;
    sf::Clock clock;

    sf::Font font;

    // Start ȭ��
    sf::Texture startBackgroundTexture;
    sf::Sprite startBackground;

    sf::RectangleShape startButtonShadow;
    sf::RectangleShape startButton;
    sf::Text startText;

    // Character selection screen
    static constexpr int CharacterCount = 6;
    std::array<sf::Texture, CharacterCount> characterTextures;
    int selectedCharacterP1;
    int selectedCharacterP2;
    bool player1Confirmed;
    bool player2Confirmed;

    sf::Text selectTitleText;
    sf::Text player1HintText;
    sf::Text player2HintText;

    // Playing ȭ��
    TileMap tileMap;
    sf::RectangleShape uiArea;
    Player player1;
    Player player2;
    std::unique_ptr<WaterBalloonSystem> player1Balloons;
    std::unique_ptr<WaterBalloonSystem> player2Balloons;

    ItemManager itemManager;
    std::array<ItemStats, 2> itemStats;
    std::array<float, 2> trappedSeconds{};
    UI hud;
    float secondsRemaining;

    // ���� ����
    int winner;

    // GameOver ȭ�� ���
    sf::Texture resultBackgroundTexture;
    sf::Sprite resultBackground;

    // ��� ȭ�� ������ �г�
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
    void beginCharacterSelect();
    void beginPlaying();
    void renderCharacterSelect();
    void createWaterBalloonSystems();
    void placeWaterBalloon(int playerIndex);
    void updateWaterBalloons(float deltaTime);
    void updateItems();
    void syncPlayerStats(int playerIndex);
    void checkGameResult();
    [[nodiscard]] sf::FloatRect getPlayerBounds(const Player& player) const;

    void centerTextOnButton(
        sf::Text& text,
        const sf::RectangleShape& button);

    void centerTextX(
        sf::Text& text,
        float y);

public:
    Game();
    ~Game();
    void run();
};
