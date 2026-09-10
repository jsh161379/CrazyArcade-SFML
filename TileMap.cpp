#include "TileMap.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <random>

namespace
{
sf::Color getTileColor(TileType type)
{
    switch (type)
    {
    case TileType::Empty:
        return sf::Color{78, 144, 82};
    case TileType::IndestructibleWall:
        return sf::Color{105, 113, 126};
    case TileType::DestructibleBlock:
        return sf::Color{184, 119, 62};
    }

    return sf::Color::Magenta;
}
}

TileMap::TileMap()
{
    reset();
}

void TileMap::reset()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> randomPercent(0, 99);

    for (int tileY = 0; tileY < Rows; ++tileY)
    {
        for (int tileX = 0; tileX < Columns; ++tileX)
        {
            const bool isBorder =
                tileX == 0 || tileX == Columns - 1 ||
                tileY == 0 || tileY == Rows - 1;

            const bool isInnerWall =
                tileX % 2 == 0 && tileY % 2 == 0;

            // 고정 벽
            if (isBorder || isInnerWall)
            {
                m_tiles[tileY][tileX] = TileType::IndestructibleWall;
            }

            // 플레이어 시작 위치 주변은 빈 공간
            else if (isSpawnSafeTile(tileX, tileY))
            {
                m_tiles[tileY][tileX] = TileType::Empty;
            }

            // 나머지 공간에서 50% 확률로 파괴 가능 블록 생성
            else if (randomPercent(gen) < 50)
            {
                m_tiles[tileY][tileX] = TileType::DestructibleBlock;
            }

            // 블록이 생성되지 않으면 빈 공간
            else
            {
                m_tiles[tileY][tileX] = TileType::Empty;
            }
        }
    }
}

void TileMap::draw(sf::RenderTarget& target) const
{
    sf::RectangleShape tileShape(
        sf::Vector2f{static_cast<float>(TileSize - 1), static_cast<float>(TileSize - 1)});

    for (int tileY = 0; tileY < Rows; ++tileY)
    {
        for (int tileX = 0; tileX < Columns; ++tileX)
        {
            tileShape.setPosition(sf::Vector2f{
                static_cast<float>(OriginX + tileX * TileSize),
                static_cast<float>(OriginY + tileY * TileSize)});
            tileShape.setFillColor(getTileColor(m_tiles[tileY][tileX]));
            target.draw(tileShape);
        }
    }
}

bool TileMap::isInside(int tileX, int tileY) const
{
    return tileX >= 0 && tileX < Columns && tileY >= 0 && tileY < Rows;
}

TileType TileMap::getTile(int tileX, int tileY) const
{
    if (!isInside(tileX, tileY))
        return TileType::IndestructibleWall;

    return m_tiles[tileY][tileX];
}

bool TileMap::isWalkable(int tileX, int tileY) const
{
    return getTile(tileX, tileY) == TileType::Empty;
}

std::optional<sf::Vector2i> TileMap::worldToTile(sf::Vector2f worldPosition) const
{
    const float localX = worldPosition.x - static_cast<float>(OriginX);
    const float localY = worldPosition.y - static_cast<float>(OriginY);

    if (localX < 0.f || localY < 0.f ||
        localX >= static_cast<float>(PixelWidth) ||
        localY >= static_cast<float>(PixelHeight))
        return std::nullopt;

    return sf::Vector2i{
        static_cast<int>(localX) / TileSize,
        static_cast<int>(localY) / TileSize};
}

bool TileMap::destroyBlock(int tileX, int tileY)
{
    if (getTile(tileX, tileY) != TileType::DestructibleBlock)
        return false;

    m_tiles[tileY][tileX] = TileType::Empty;
    return true;
}

bool TileMap::isSpawnSafeTile(int tileX, int tileY)
{
    const bool isTopLeftSpawn =
        (tileX == 1 && tileY == 1) ||
        (tileX == 2 && tileY == 1) ||
        (tileX == 1 && tileY == 2);

    const bool isBottomRightSpawn =
        (tileX == Columns - 2 && tileY == Rows - 2) ||
        (tileX == Columns - 3 && tileY == Rows - 2) ||
        (tileX == Columns - 2 && tileY == Rows - 3);

    return isTopLeftSpawn || isBottomRightSpawn;
}
