#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>

#include <array>
#include <optional>

enum class TileType
{
    Empty,
    IndestructibleWall,
    DestructibleBlock
};

class TileMap
{
public:
    static constexpr int Columns = 15;
    static constexpr int Rows = 13;
    static constexpr int TileSize = 40;
    static constexpr int OriginX = 0;
    static constexpr int OriginY = 40;
    static constexpr int PixelWidth = Columns * TileSize;
    static constexpr int PixelHeight = Rows * TileSize;

    TileMap();

    void reset();
    void draw(sf::RenderTarget& target) const;

    [[nodiscard]] bool isInside(int tileX, int tileY) const;
    [[nodiscard]] TileType getTile(int tileX, int tileY) const;
    [[nodiscard]] bool isWalkable(int tileX, int tileY) const;
    [[nodiscard]] std::optional<sf::Vector2i> worldToTile(sf::Vector2f worldPosition) const;

    bool destroyBlock(int tileX, int tileY);

private:
    using TileRow = std::array<TileType, Columns>;
    std::array<TileRow, Rows> m_tiles{};

    [[nodiscard]] static bool isSpawnSafeTile(int tileX, int tileY);
};

