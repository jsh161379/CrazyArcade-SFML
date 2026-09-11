#pragma once

#include "TileArt.h"
#include <SFML/Graphics/Texture.hpp>
#include <array>
#include <filesystem>
#include <optional>

// Gameplay rules are independent of the drawing style.
enum class TileType { Empty, IndestructibleWall, DestructibleBlock };

struct Tile
{
    GroundStyle ground = GroundStyle::Grass;
    ObjectStyle object = ObjectStyle::None;
};

class TileMap
{
public:
    static constexpr int Columns = 15;
    static constexpr int Rows = 13;
    static constexpr int TileSize = 56;
    static constexpr int OriginX = 12;
    static constexpr int OriginY = 8;
    static constexpr int PixelWidth = Columns * TileSize;
    static constexpr int PixelHeight = Rows * TileSize;
    static constexpr int SpawnX = 0;
    static constexpr int SpawnY = 1;

    explicit TileMap(const std::filesystem::path& assetDirectory = "assets");
    void reset();
    void draw(sf::RenderTarget& target) const;
    void drawGround(sf::RenderTarget& target) const;
    void drawObjectRow(sf::RenderTarget& target, int row) const;

    [[nodiscard]] bool isInside(int x, int y) const;
    [[nodiscard]] TileType getTile(int x, int y) const;
    [[nodiscard]] ObjectStyle getObject(int x, int y) const;
    [[nodiscard]] GroundStyle getGround(int x, int y) const;
    [[nodiscard]] bool isWalkable(int x, int y) const;
    [[nodiscard]] std::optional<sf::Vector2i> worldToTile(sf::Vector2f worldPosition) const;
    bool destroyBlock(int x, int y);

private:
    std::array<std::array<Tile, Columns>, Rows> m_tiles{};
    // Textures live as long as the map. Missing PNGs use the code-drawn art.
    std::array<std::optional<sf::Texture>, 4> m_groundTextures;
    std::array<std::optional<sf::Texture>, 9> m_objectTextures;
    void loadTextures(const std::filesystem::path& assetDirectory);
};
