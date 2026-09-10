#include "Game.h"
#include <algorithm>
#include <iostream>

using namespace sf;
using namespace std;


Game::Game()
    : window(VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }), "CrazyArcade"),
    state(GameState::Start),
    winner(0),
    startBackground(startBackgroundTexture),
    startText(font),
    gameOverText(font),
    winnerText(font),
    restartText(font),
    exitText(font)
{
    // -----------------------------
    // 폰트
    // -----------------------------
    if (!font.openFromFile("assets/fonts/neodgm.ttf"))
    {
        cout << "font load failed!" << endl;
    }

    font.setSmooth(false);


    // -----------------------------
    // 창 크기
    // -----------------------------
    float windowWidth = static_cast<float>(WINDOW_WIDTH);
    float windowHeight = static_cast<float>(WINDOW_HEIGHT);


    // -----------------------------
    // 실제 맵 크기
    // -----------------------------
    float mapWidth = MAP_COLS * TILE_SIZE;      // 15 * 40 = 600
    float mapHeight = MAP_ROWS * TILE_SIZE;    // 13 * 40 = 520

    // 맵 오른쪽에 남는 UI 영역 너비
    float uiWidth = windowWidth - mapWidth;     // 800 - 600 = 200


    // -----------------------------
    // 시작 화면 배경
    // -----------------------------
    if (!startBackgroundTexture.loadFromFile(
        "assets/images/start_background.png"))
    {
        cout << "start_background.png load failed!" << endl;
    }
    else
    {
        cout << "start_background.png load success!" << endl;

        startBackground.setTexture(startBackgroundTexture, true);

        Vector2u windowSize = window.getSize();
        Vector2u textureSize = startBackgroundTexture.getSize();

        float scaleX =
            static_cast<float>(windowSize.x) /
            static_cast<float>(textureSize.x);

        float scaleY =
            static_cast<float>(windowSize.y) /
            static_cast<float>(textureSize.y);

        // 이미지 비율을 유지하면서 화면 전체 채우기
        float scale = std::max(scaleX, scaleY);

        startBackground.setScale({ scale, scale });

        float backgroundWidth =
            static_cast<float>(textureSize.x) * scale;

        float backgroundHeight =
            static_cast<float>(textureSize.y) * scale;

        float backgroundX =
            (static_cast<float>(windowSize.x) - backgroundWidth) / 2.f;

        float backgroundY =
            (static_cast<float>(windowSize.y) - backgroundHeight) / 2.f;

        startBackground.setPosition({
            backgroundX,
            backgroundY
            });
    }


    // -----------------------------
    // 게임 시작 버튼
    // -----------------------------
    startButton.setSize({ 240.f, 70.f });

    // 화면 가로 가운데
    float startButtonX =
        (windowWidth - startButton.getSize().x) / 2.f;

    startButton.setPosition({ startButtonX, 420.f });

    startButton.setFillColor(Color(30, 130, 230));
    startButton.setOutlineColor(Color::White);
    startButton.setOutlineThickness(4.f);


    // 시작 버튼 그림자
    startButtonShadow.setSize(startButton.getSize());

    startButtonShadow.setPosition({
        startButtonX,
        startButton.getPosition().y + 8.f
        });

    startButtonShadow.setFillColor(Color(0, 70, 150));


    // 시작 버튼 글자
    startText.setString(L"게임 시작");
    startText.setCharacterSize(36);

    centerTextOnButton(startText, startButton);


    // -----------------------------
    // Playing 임시 화면
    // -----------------------------

    // 실제 맵의 가로 크기는 600
    // 아직 아래쪽 UI가 확정되지 않았으므로
    // 임시 색상 영역은 창 높이 전체를 채움
    mapArea.setSize({
        mapWidth,
        windowHeight
        });

    mapArea.setFillColor(Color(80, 180, 220));
    mapArea.setPosition({ 0.f, 0.f });


    // 오른쪽 UI 영역
    uiArea.setSize({
        uiWidth,
        windowHeight
        });

    uiArea.setFillColor(Color(230, 230, 230));
    uiArea.setPosition({
        mapWidth,
        0.f
        });


    // -----------------------------
    // GameOver 화면
    // -----------------------------
    gameOverText.setString("GAME OVER");
    gameOverText.setCharacterSize(60);

    centerTextX(gameOverText, 150.f);


    // 승자 글씨
    winnerText.setString(L"승자 없음");
    winnerText.setCharacterSize(36);

    centerTextX(winnerText, 250.f);


    // -----------------------------
    // 다시 시작 버튼
    // -----------------------------
    restartButton.setSize({ 240.f, 70.f });

    float restartButtonX =
        (windowWidth - restartButton.getSize().x) / 2.f;

    restartButton.setPosition({
        restartButtonX,
        350.f
        });

    restartButton.setFillColor(Color(30, 130, 230));
    restartButton.setOutlineColor(Color::White);
    restartButton.setOutlineThickness(4.f);


    restartText.setString(L"다시 시작");
    restartText.setCharacterSize(36);

    centerTextOnButton(restartText, restartButton);


    // -----------------------------
    // 게임 종료 버튼
    // -----------------------------
    exitButton.setSize({ 240.f, 70.f });

    float exitButtonX =
        (windowWidth - exitButton.getSize().x) / 2.f;

    exitButton.setPosition({
        exitButtonX,
        450.f
        });

    exitButton.setFillColor(Color(200, 70, 70));
    exitButton.setOutlineColor(Color::White);
    exitButton.setOutlineThickness(4.f);


    exitText.setString(L"게임 종료");
    exitText.setCharacterSize(36);

    centerTextOnButton(exitText, exitButton);
}


void Game::run()
{
    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();

        processEvents();
        update(deltaTime);
        render();
    }
}


void Game::processEvents()
{
    while (const auto event = window.pollEvent())
    {
        if (event->is<Event::Closed>())
        {
            window.close();
        }

        if (const auto* mousePressed =
            event->getIf<Event::MouseButtonPressed>())
        {
            if (mousePressed->button == Mouse::Button::Left)
            {
                Vector2f mousePosition(mousePressed->position);

                if (state == GameState::Start)
                {
                    if (startButton
                        .getGlobalBounds()
                        .contains(mousePosition))
                    {
                        state = GameState::Playing;
                    }
                }
                else if (state == GameState::GameOver)
                {
                    if (restartButton
                        .getGlobalBounds()
                        .contains(mousePosition))
                    {
                        resetGame();
                    }
                    else if (exitButton
                        .getGlobalBounds()
                        .contains(mousePosition))
                    {
                        window.close();
                    }
                }
            }
        }

        if (const auto* keyPressed =
            event->getIf<Event::KeyPressed>())
        {
            if (state == GameState::Playing)
            {
                if (keyPressed->scancode ==
                    Keyboard::Scancode::Escape)
                {
                    setWinner(1);
                    state = GameState::GameOver;
                }
            }
        }
    }
}


void Game::update(float deltaTime)
{
    if (state == GameState::Start)
    {
        Vector2i mousePixel =
            Mouse::getPosition(window);

        Vector2f mousePosition =
            window.mapPixelToCoords(mousePixel);

        if (startButton
            .getGlobalBounds()
            .contains(mousePosition))
        {
            startButton.setFillColor(
                Color(70, 170, 255));
        }
        else
        {
            startButton.setFillColor(
                Color(30, 130, 230));
        }
    }
    else if (state == GameState::Playing)
    {
        // 실제 게임 진행
    }
    else if (state == GameState::GameOver)
    {
        Vector2i mousePixel =
            Mouse::getPosition(window);

        Vector2f mousePosition =
            window.mapPixelToCoords(mousePixel);


        // 다시 시작 버튼 Hover
        if (restartButton
            .getGlobalBounds()
            .contains(mousePosition))
        {
            restartButton.setFillColor(
                Color(70, 170, 255));
        }
        else
        {
            restartButton.setFillColor(
                Color(30, 130, 230));
        }


        // 게임 종료 버튼 Hover
        if (exitButton
            .getGlobalBounds()
            .contains(mousePosition))
        {
            exitButton.setFillColor(
                Color(235, 90, 90));
        }
        else
        {
            exitButton.setFillColor(
                Color(200, 70, 70));
        }
    }
}


void Game::render()
{
    if (state == GameState::Start)
    {
        window.clear();

        window.draw(startBackground);

        window.draw(startButtonShadow);
        window.draw(startButton);
        window.draw(startText);
    }
    else if (state == GameState::Playing)
    {
        window.clear();

        window.draw(mapArea);
        window.draw(uiArea);
    }
    else if (state == GameState::GameOver)
    {
        window.clear(Color(40, 40, 60));

        window.draw(gameOverText);
        window.draw(winnerText);

        window.draw(restartButton);
        window.draw(restartText);

        window.draw(exitButton);
        window.draw(exitText);
    }

    window.display();
}


void Game::resetGame()
{
    winner = 0;
    state = GameState::Playing;
}


// ------------------------------------------------
// 버튼 안에 글씨 중앙 정렬
// ------------------------------------------------
void Game::centerTextOnButton(
    Text& text,
    const RectangleShape& button)
{
    FloatRect textBounds = text.getLocalBounds();
    FloatRect buttonBounds = button.getGlobalBounds();

    text.setPosition({
        buttonBounds.position.x +
        (buttonBounds.size.x - textBounds.size.x) / 2.f -
        textBounds.position.x,

        buttonBounds.position.y +
        (buttonBounds.size.y - textBounds.size.y) / 2.f -
        textBounds.position.y
        });
}


// ------------------------------------------------
// 화면 가로 기준 글씨 중앙 정렬
// ------------------------------------------------
void Game::centerTextX(Text& text, float y)
{
    FloatRect textBounds = text.getLocalBounds();

    float windowWidth =
        static_cast<float>(window.getSize().x);

    float x =
        (windowWidth - textBounds.size.x) / 2.f -
        textBounds.position.x;

    text.setPosition({ x, y });
}

void Game::setWinner(int playerNumber)
{
    winner = playerNumber;

    if (winner == 1)
    {
        winnerText.setString(L"1P 승리!");
    }
    else if (winner == 2)
    {
        winnerText.setString(L"2P 승리!");
    }
    else
    {
        winnerText.setString(L"승자 없음");
    }

    centerTextX(winnerText, 250.f);
}