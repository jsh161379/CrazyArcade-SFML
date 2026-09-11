#include "Player.h"

#include "TileMap.h"

int dirRow(const std::string& dir)
{
    if (dir == "down") return 0;
    if (dir == "up") return 1;
    if (dir == "left") return 2;
    if (dir == "right") return 3;
    return 0;
}

bool Player::loadTexture(const std::string& path)
{
    textureLoaded = texture.loadFromFile(path);
    if (textureLoaded)
        texture.setSmooth(false);
    return textureLoaded;
}

void Player::draw(sf::RenderWindow& window) const
{
    if (!textureLoaded || !alive)
        return;

    const sf::Vector2u textureSize = texture.getSize();
    const int frameWidth = static_cast<int>(textureSize.x) / PlayerFrameCount;
    const int frameHeight = static_cast<int>(textureSize.y) / 4;
    if (frameWidth <= 0 || frameHeight <= 0)
        return;

    sf::Sprite sprite(texture);
    sprite.setTextureRect(sf::IntRect(
        { currentFrame * frameWidth, dirRow(dir) * frameHeight },
        { frameWidth, frameHeight }));
    sprite.setOrigin({ frameWidth / 2.f, frameHeight / 2.f });

    const float scale = drawSize / static_cast<float>(frameHeight);
    sprite.setScale({ scale, scale });
    sprite.setPosition(getRenderPos(*this));
    window.draw(sprite);
}

void resetPlayer(Player& player, int row, int col)
{
    player.row = row;
    player.col = col;
    player.targetRow = row;
    player.targetCol = col;
    player.moving = false;
    player.progress = 0.f;
    player.alive = true;
    player.trapped = false;
    player.bombCapacity = 1;
    player.blastRange = 1;
    player.needles = 0;
    player.dir = "down";
    player.currentFrame = 0;
    player.animTimer = 0.f;
}

void updatePlayer(Player& player, const TileMap& map, float deltaTime)
{
    if (!player.alive || player.trapped)
    {
        player.moving = false;
        player.currentFrame = 0;
        player.animTimer = 0.f;
        return;
    }

    if (player.moving)
    {
        player.progress += player.speed * deltaTime;
        player.animTimer += deltaTime;

        if (player.animTimer >= player.animSpeed)
        {
            player.animTimer -= player.animSpeed;
            player.currentFrame = (player.currentFrame + 1) % PlayerFrameCount;
        }

        if (player.progress >= 1.f)
        {
            player.row = player.targetRow;
            player.col = player.targetCol;
            player.moving = false;
            player.progress = 0.f;
            player.currentFrame = 0;
            player.animTimer = 0.f;
        }
        return;
    }

    int deltaColumn = 0;
    int deltaRow = 0;

    if (sf::Keyboard::isKeyPressed(player.keyUp))
    {
        deltaRow = -1;
        player.dir = "up";
    }
    else if (sf::Keyboard::isKeyPressed(player.keyDown))
    {
        deltaRow = 1;
        player.dir = "down";
    }
    else if (sf::Keyboard::isKeyPressed(player.keyLeft))
    {
        deltaColumn = -1;
        player.dir = "left";
    }
    else if (sf::Keyboard::isKeyPressed(player.keyRight))
    {
        deltaColumn = 1;
        player.dir = "right";
    }

    if (deltaColumn == 0 && deltaRow == 0)
    {
        player.currentFrame = 0;
        player.animTimer = 0.f;
        return;
    }

    const int nextColumn = player.col + deltaColumn;
    const int nextRow = player.row + deltaRow;
    if (map.isWalkable(nextColumn, nextRow))
    {
        player.targetCol = nextColumn;
        player.targetRow = nextRow;
        player.moving = true;
        player.progress = 0.f;
        player.currentFrame = 0;
        player.animTimer = 0.f;
    }
}

sf::Vector2f getRenderPos(const Player& player)
{
    float renderRow = static_cast<float>(player.row);
    float renderColumn = static_cast<float>(player.col);

    if (player.moving)
    {
        renderRow += static_cast<float>(player.targetRow - player.row) * player.progress;
        renderColumn += static_cast<float>(player.targetCol - player.col) * player.progress;
    }

    return {
        static_cast<float>(TileMap::OriginX) + (renderColumn + 0.5f) * TileMap::TileSize,
        static_cast<float>(TileMap::OriginY) + (renderRow + 0.5f) * TileMap::TileSize
    };
}

sf::Vector2i getTilePosition(const Player& player)
{
    return { player.col, player.row };
}

void killPlayer(Player& player)
{
    player.alive = false;
    player.moving = false;
    player.progress = 0.f;
    player.currentFrame = 0;
    player.animTimer = 0.f;
}

bool isAlive(const Player& player)
{
    return player.alive;
}
