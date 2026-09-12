#include "Player.h"
#include "TileMap.h"
#include <cmath>
#include <string>
#include <unordered_map>
#include <memory>

static std::unordered_map<const Player*, std::unique_ptr<sf::Texture>> trappedTextures;

static std::string makeTrappedPath(const std::string& path)
{
    std::string result = path;
    size_t pos = result.rfind(".png");
    if (pos != std::string::npos) result.insert(pos, "_trapped");
    return result;
}

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
    if (textureLoaded) texture.setSmooth(false);

    std::string trappedPath = makeTrappedPath(path);
    auto trappedTexture = std::make_unique<sf::Texture>();

    if (trappedTexture->loadFromFile(trappedPath))
    {
        trappedTexture->setSmooth(false);
        trappedTextures[this] = std::move(trappedTexture);
    }
    else trappedTextures.erase(this);

    return textureLoaded;
}

static void drawNormalPlayer(const Player& player, sf::RenderWindow& window)
{
    const sf::Vector2u textureSize = player.texture.getSize();
    const int frameWidth = static_cast<int>(textureSize.x) / PlayerFrameCount;
    const int frameHeight = static_cast<int>(textureSize.y) / 4;
    if (frameWidth <= 0 || frameHeight <= 0) return;

    sf::Sprite sprite(player.texture);
    sprite.setTextureRect(sf::IntRect(
        { player.currentFrame * frameWidth,dirRow(player.dir) * frameHeight },
        { frameWidth,frameHeight }
    ));
    sprite.setOrigin({ frameWidth / 2.f,frameHeight / 2.f });

    float scale = player.drawSize / static_cast<float>(frameHeight);
    sprite.setScale({ scale,scale });
    sprite.setPosition(getRenderPos(player));
    window.draw(sprite);
}

static void drawBubbleBack(const Player& player, sf::RenderWindow& window)
{
    sf::Vector2f pos = getRenderPos(player);

    float ratio = player.trappedAnimTimer / Player::TrapDuration;
    if (ratio < 0.f) ratio = 0.f;
    if (ratio > 1.f) ratio = 1.f;

    float appear = player.trappedAnimTimer / 0.25f;
    if (appear > 1.f) appear = 1.f;

    float pulseSpeed = 6.f;
    float pulsePower = 1.2f;

    if (ratio > 0.7f)
    {
        pulseSpeed = 10.f;
        pulsePower = 2.f;
    }

    if (ratio > 0.88f)
    {
        pulseSpeed = 22.f;
        pulsePower = 3.f;
    }

    float radius = (25.f + std::sin(player.trappedAnimTimer * pulseSpeed) * pulsePower) * appear;

    int alpha = static_cast<int>(45.f + 125.f * ratio);
    if (alpha > 180) alpha = 180;

    sf::CircleShape bubble(radius);
    bubble.setOrigin({ radius,radius });
    bubble.setPosition(pos);
    bubble.setFillColor(sf::Color(60, 180, 250, static_cast<unsigned char>(alpha)));
    window.draw(bubble);
}

static void drawTrappedPlayer(const Player& player, sf::RenderWindow& window)
{
    auto it = trappedTextures.find(&player);

    if (it == trappedTextures.end())
    {
        drawNormalPlayer(player, window);
        return;
    }

    const sf::Texture& trappedTexture = *it->second;
    const sf::Vector2u textureSize = trappedTexture.getSize();
    const int frameWidth = static_cast<int>(textureSize.x) / PlayerFrameCount;
    const int frameHeight = static_cast<int>(textureSize.y);

    if (frameWidth <= 0 || frameHeight <= 0)
    {
        drawNormalPlayer(player, window);
        return;
    }

    float ratio = player.trappedAnimTimer / Player::TrapDuration;
    if (ratio < 0.f) ratio = 0.f;
    if (ratio > 1.f) ratio = 1.f;

    sf::Vector2f pos = getRenderPos(player);

    float shakePower = 1.f;
    float shakeSpeed = 13.f;

    if (ratio > 0.65f)
    {
        shakePower = 2.f;
        shakeSpeed = 18.f;
    }

    if (ratio > 0.88f)
    {
        shakePower = 3.5f;
        shakeSpeed = 28.f;
    }

    pos.x += std::sin(player.trappedAnimTimer * shakeSpeed) * shakePower;
    pos.y += std::sin(player.trappedAnimTimer * 11.f);

    sf::Sprite sprite(trappedTexture);
    sprite.setTextureRect(sf::IntRect(
        { player.currentFrame * frameWidth,0 },
        { frameWidth,frameHeight }
    ));
    sprite.setOrigin({ frameWidth / 2.f,frameHeight / 2.f });

    float scale = 52.f / static_cast<float>(frameHeight);
    scale *= 1.f + std::sin(player.trappedAnimTimer * 8.f) * 0.015f;

    sprite.setScale({ scale,scale });
    sprite.setPosition(pos);
    window.draw(sprite);
}

static void drawBubbleFront(const Player& player, sf::RenderWindow& window)
{
    sf::Vector2f pos = getRenderPos(player);

    float ratio = player.trappedAnimTimer / Player::TrapDuration;
    if (ratio < 0.f) ratio = 0.f;
    if (ratio > 1.f) ratio = 1.f;

    float appear = player.trappedAnimTimer / 0.25f;
    if (appear > 1.f) appear = 1.f;

    float pulseSpeed = 6.f;
    float pulsePower = 1.2f;

    if (ratio > 0.7f)
    {
        pulseSpeed = 10.f;
        pulsePower = 2.f;
    }

    if (ratio > 0.88f)
    {
        pulseSpeed = 22.f;
        pulsePower = 3.f;
    }

    float radius = (25.f + std::sin(player.trappedAnimTimer * pulseSpeed) * pulsePower) * appear;

    int outlineAlpha = static_cast<int>(140.f + 100.f * ratio);
    if (outlineAlpha > 255) outlineAlpha = 255;

    sf::CircleShape outline(radius);
    outline.setOrigin({ radius,radius });
    outline.setPosition(pos);
    outline.setFillColor(sf::Color::Transparent);
    outline.setOutlineThickness(2.f);
    outline.setOutlineColor(sf::Color(
        170, 235, 255,
        static_cast<unsigned char>(outlineAlpha)
    ));
    window.draw(outline);

    sf::CircleShape highlight(radius * 0.17f);
    highlight.setOrigin({ radius * 0.17f,radius * 0.17f });
    highlight.setPosition({
        pos.x - radius * 0.37f,
        pos.y - radius * 0.39f
        });
    highlight.setFillColor(sf::Color(255, 255, 255, 180));
    window.draw(highlight);

    for (int i = 0; i < 3; ++i)
    {
        float smallRadius = 1.8f + i * 0.7f;
        float phase = player.trappedAnimTimer * (1.5f + i * 0.4f) + i * 2.f;
        float rise = std::fmod(
            player.trappedAnimTimer * (9.f + i * 3.f) + i * 9.f,
            radius * 1.25f
        );

        sf::CircleShape smallBubble(smallRadius);
        smallBubble.setOrigin({ smallRadius,smallRadius });
        smallBubble.setPosition({
            pos.x + std::sin(phase) * radius * 0.35f,
            pos.y + radius * 0.38f - rise
            });
        smallBubble.setFillColor(sf::Color(215, 245, 255, 150));
        window.draw(smallBubble);
    }

    if (ratio > 0.88f)
    {
        float warning = (ratio - 0.88f) / 0.12f;
        if (warning > 1.f) warning = 1.f;

        int warningAlpha = static_cast<int>(warning * 130.f);

        sf::CircleShape warningRing(radius + 3.f);
        warningRing.setOrigin({ radius + 3.f,radius + 3.f });
        warningRing.setPosition(pos);
        warningRing.setFillColor(sf::Color::Transparent);
        warningRing.setOutlineThickness(2.f);
        warningRing.setOutlineColor(sf::Color(
            255, 255, 255,
            static_cast<unsigned char>(warningAlpha)
        ));
        window.draw(warningRing);
    }
}

void Player::draw(sf::RenderWindow& window) const
{
    if (!textureLoaded || !alive) return;

    if (!trapped)
    {
        drawNormalPlayer(*this, window);
        return;
    }

    drawBubbleBack(*this, window);
    drawTrappedPlayer(*this, window);
    drawBubbleFront(*this, window);
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
    player.trappedAnimTimer = 0.f;
    player.bombCapacity = 1;
    player.blastRange = 1;
    player.needles = 0;
    player.dir = "down";
    player.currentFrame = 0;
    player.animTimer = 0.f;
}

void updatePlayer(Player& player, const TileMap& map, float deltaTime, const std::function<bool(sf::Vector2i)>& isBalloonAt)
{
    if (!player.alive)
    {
        player.moving = false;
        player.currentFrame = 0;
        player.animTimer = 0.f;
        return;
    }

    if (player.trapped)
    {
        player.moving = false;
        player.trappedAnimTimer += deltaTime;
        player.animTimer += deltaTime;

        float trappedAnimSpeed = 0.11f;

        if (player.trappedAnimTimer > Player::TrapDuration * 0.7f)
            trappedAnimSpeed = 0.085f;

        if (player.trappedAnimTimer > Player::TrapDuration * 0.88f)
            trappedAnimSpeed = 0.06f;

        if (player.animTimer >= trappedAnimSpeed)
        {
            player.animTimer -= trappedAnimSpeed;
            player.currentFrame = (player.currentFrame + 1) % PlayerFrameCount;
        }

        return;
    }

    player.trappedAnimTimer = 0.f;

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
    sf::Vector2i nextTile = { nextColumn,nextRow };

    bool balloonBlocked = false;
    if (isBalloonAt) balloonBlocked = isBalloonAt(nextTile);

    if (map.isWalkable(nextColumn, nextRow) && !balloonBlocked)
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
    return { player.col,player.row };
}

void killPlayer(Player& player)
{
    player.alive = false;
    player.trapped = false;
    player.moving = false;
    player.progress = 0.f;
    player.trappedAnimTimer = 0.f;
    player.currentFrame = 0;
    player.animTimer = 0.f;
}

bool isAlive(const Player& player)
{
    return player.alive;
}