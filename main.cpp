#include "TileMap.h"

#include <SFML/Graphics.hpp>

int main()
{
    constexpr unsigned int WindowWidth = 800;
    constexpr unsigned int WindowHeight = 600;

    sf::RenderWindow window(
        sf::VideoMode({ WindowWidth, WindowHeight }),
        L"Crazy Arcade - Map and Block Test");
    window.setFramerateLimit(60);

    TileMap tileMap;

    sf::RectangleShape sidePanel(sf::Vector2f{ 200.f, 600.f });
    sidePanel.setPosition(sf::Vector2f{ 600.f, 0.f });
    sidePanel.setFillColor(sf::Color{ 35, 42, 55 });

    // =============================
    // 임시 플레이어 테스트
    // =============================
    int playerTileX = 1;
    int playerTileY = 1;

    sf::RectangleShape player(sf::Vector2f{ 30.f, 30.f });
    player.setFillColor(sf::Color::Blue);

    auto updatePlayerPosition = [&]()
        {
            player.setPosition(sf::Vector2f{
                static_cast<float>(TileMap::OriginX + playerTileX * TileMap::TileSize + 5),
                static_cast<float>(TileMap::OriginY + playerTileY * TileMap::TileSize + 5)
                });
        };

    updatePlayerPosition();

    while (window.isOpen())
    {
        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            else if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
            {
                // 테스트용: 갈색 파괴 가능 블록 클릭 시 제거
                if (mousePressed->button == sf::Mouse::Button::Left)
                {
                    const sf::Vector2f worldPosition =
                        window.mapPixelToCoords(mousePressed->position);

                    if (const auto tile = tileMap.worldToTile(worldPosition))
                        tileMap.destroyBlock(tile->x, tile->y);
                }
            }
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                int nextX = playerTileX;
                int nextY = playerTileY;

                // 이동하려는 타일 좌표 계산
                if (keyPressed->code == sf::Keyboard::Key::Left)
                {
                    nextX--;
                }
                else if (keyPressed->code == sf::Keyboard::Key::Right)
                {
                    nextX++;
                }
                else if (keyPressed->code == sf::Keyboard::Key::Up)
                {
                    nextY--;
                }
                else if (keyPressed->code == sf::Keyboard::Key::Down)
                {
                    nextY++;
                }
                else if (keyPressed->code == sf::Keyboard::Key::R)
                {
                    // 테스트용: 맵 초기화 후 플레이어도 시작 위치로 복귀
                    tileMap.reset();
                    playerTileX = 1;
                    playerTileY = 1;
                    updatePlayerPosition();
                    continue;
                }

                // 빈 타일일 때만 이동
                if (tileMap.isWalkable(nextX, nextY))
                {
                    playerTileX = nextX;
                    playerTileY = nextY;
                    updatePlayerPosition();
                }
            }
        }

        window.clear(sf::Color{ 25, 30, 38 });

        tileMap.draw(window);
        window.draw(player);
        window.draw(sidePanel);

        window.display();
    }

    return 0;
}



























//#include "TileMap.h"
//
//#include <SFML/Graphics.hpp>
//
//int main()
//{
//    constexpr unsigned int WindowWidth = 800;
//    constexpr unsigned int WindowHeight = 600;
//
//    sf::RenderWindow window(
//        sf::VideoMode({WindowWidth, WindowHeight}),
//        L"Crazy Arcade - Map and Block Test");
//    window.setFramerateLimit(60);
//
//    TileMap tileMap;
//
//    sf::RectangleShape sidePanel(sf::Vector2f{200.f, 600.f});
//    sidePanel.setPosition(sf::Vector2f{600.f, 0.f});
//    sidePanel.setFillColor(sf::Color{35, 42, 55});
//
//    while (window.isOpen())
//    {
//        while (const auto event = window.pollEvent())
//        {
//            if (event->is<sf::Event::Closed>())
//            {
//                window.close();
//            }
//            else if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
//            {
//                if (mousePressed->button == sf::Mouse::Button::Left)
//                {
//                    const sf::Vector2f worldPosition = window.mapPixelToCoords(mousePressed->position);
//                    if (const auto tile = tileMap.worldToTile(worldPosition))
//                        tileMap.destroyBlock(tile->x, tile->y);
//                }
//            }
//            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
//            {
//                if (keyPressed->code == sf::Keyboard::Key::R)
//                    tileMap.reset();
//            }
//        }
//
//        window.clear(sf::Color{25, 30, 38});
//        tileMap.draw(window);
//        window.draw(sidePanel);
//        window.display();
//    }
//
//    return 0;
//}
