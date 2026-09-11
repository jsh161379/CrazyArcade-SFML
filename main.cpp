#include "TileMap.h"
#include <SFML/Graphics.hpp>
#include <filesystem>
#include <iostream>
#include <string_view>
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace
{
constexpr unsigned int WindowWidth = TileMap::PixelWidth + TileMap::OriginX * 2;
constexpr unsigned int WindowHeight = TileMap::PixelHeight + TileMap::OriginY * 2;

std::filesystem::path executableDirectory()
{
    wchar_t path[32768]{};
    const auto length = GetModuleFileNameW(nullptr, path, 32768);
    if (length == 0 || length >= 32768) return std::filesystem::current_path();
    return std::filesystem::path(path).parent_path();
}

// A simple test pawn; the screenshot's character artwork is not included.
void drawPlayer(sf::RenderTarget& target, sf::Vector2i tile)
{
    const float size = static_cast<float>(TileMap::TileSize);
    const float x = TileMap::OriginX + tile.x * size;
    const float y = TileMap::OriginY + tile.y * size;
    auto ellipse = [&](float dx, float dy, float width, float height, sf::Color color)
    {
        sf::CircleShape shape(1.f, 32);
        shape.setScale({width * size / 2.f, height * size / 2.f});
        shape.setPosition({x + dx * size, y + dy * size});
        shape.setFillColor(color);
        target.draw(shape);
    };
    ellipse(.15f, .76f, .7f, .2f, {15, 60, 44, 90});
    ellipse(.20f, .67f, .28f, .24f, {242, 250, 255});
    ellipse(.52f, .67f, .28f, .24f, {242, 250, 255});
    ellipse(.26f, .43f, .48f, .4f, {20, 120, 219});
    ellipse(.12f, -.12f, .76f, .76f, {14, 100, 190});
    ellipse(.17f, -.12f, .62f, .62f, {48, 187, 251});
    ellipse(.26f, .10f, .48f, .40f, {255, 222, 171});
    ellipse(.36f, .22f, .045f, .11f, {31, 52, 72});
    ellipse(.60f, .22f, .045f, .11f, {31, 52, 72});
    ellipse(.46f, .38f, .10f, .025f, {157, 79, 63});
    ellipse(.27f, -.06f, .22f, .09f, {187, 236, 255});
    ellipse(.43f, .59f, .14f, .14f, {245, 224, 103});
}

void drawScene(sf::RenderTarget& target, const TileMap& map, sf::Vector2i player)
{
    sf::RectangleShape border({static_cast<float>(TileMap::PixelWidth + 8),
                              static_cast<float>(TileMap::PixelHeight + 8)});
    border.setPosition({static_cast<float>(TileMap::OriginX - 4),
                        static_cast<float>(TileMap::OriginY - 4)});
    border.setFillColor({64, 81, 91});
    target.draw(border);
    map.drawGround(target);
    for (int row = 0; row < TileMap::Rows; ++row)
    {
        map.drawObjectRow(target, row);
        if (player.y == row) drawPlayer(target, player);
    }
}
}

int main(int argc, char** argv)
{
    TileMap map(executableDirectory() / "assets");
    sf::Vector2i player{TileMap::SpawnX, TileMap::SpawnY};
    // Produce the same scene without opening an interactive game window.
    if (argc == 3 && std::string_view(argv[1]) == "--preview")
    {
        sf::RenderTexture preview({WindowWidth, WindowHeight});
        preview.clear({25, 39, 48});
        drawScene(preview, map, player);
        preview.display();
        if (!preview.getTexture().copyToImage().saveToFile(argv[2])) return 1;
        std::cout << "Preview saved.\n";
        return 0;
    }

    sf::RenderWindow window(sf::VideoMode({WindowWidth, WindowHeight}),
                            L"",
                            sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);
    while (window.isOpen())
    {
        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) window.close();
            else if (const auto* mouse = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mouse->button == sf::Mouse::Button::Left)
                    if (const auto tile = map.worldToTile(window.mapPixelToCoords(mouse->position)))
                        map.destroyBlock(tile->x, tile->y);
            }
            else if (const auto* key = event->getIf<sf::Event::KeyPressed>())
            {
                auto next = player;
                switch (key->code)
                {
                case sf::Keyboard::Key::Left: --next.x; break;
                case sf::Keyboard::Key::Right: ++next.x; break;
                case sf::Keyboard::Key::Up: --next.y; break;
                case sf::Keyboard::Key::Down: ++next.y; break;
                case sf::Keyboard::Key::R:
                    map.reset();
                    next = {TileMap::SpawnX, TileMap::SpawnY};
                    break;
                default: break;
                }
                if (map.isWalkable(next.x, next.y)) player = next;
            }
        }
        if (!window.isOpen()) break;
        window.clear({25, 39, 48});
        drawScene(window, map, player);
        window.display();
    }
}
