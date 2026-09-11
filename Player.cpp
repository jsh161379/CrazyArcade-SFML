#include "Player.h"
#include "TileMap.h"


// ============================================================
// 방향 -> 스프라이트 시트 행
//
// 현재 스프라이트 시트:
// 0 : 아래
// 1 : 위
// 2 : 왼쪽
// 3 : 오른쪽
// ============================================================

int dirRow(const std::string& dir)
{
    if (dir == "down")
        return 0;

    if (dir == "up")
        return 1;

    if (dir == "left")
        return 2;

    if (dir == "right")
        return 3;

    return 0;
}


// ============================================================
// Texture 로드
// ============================================================

bool Player::loadTexture(const std::string& path)
{
    if (!texture.loadFromFile(path))
    {
        textureLoaded = false;
        return false;
    }

    textureLoaded = true;
    return true;
}


// ============================================================
// Player 출력
// ============================================================

void Player::draw(sf::RenderWindow& window) const
{
    if (!textureLoaded)
        return;

    if (!alive)
        return;


    sf::Sprite sprite(texture);


    // --------------------------------------------------------
    // 현재 방향 + 현재 프레임
    // --------------------------------------------------------

    sprite.setTextureRect(
        sf::IntRect(
            {
                currentFrame * FRAME_W,
                dirRow(dir) * FRAME_H
            },
            {
                FRAME_W,
                FRAME_H
            }
        )
    );


    // --------------------------------------------------------
    // 스프라이트 중심점
    // --------------------------------------------------------

    sprite.setOrigin(
        {
            FRAME_W / 2.f,
            FRAME_H / 2.f
        }
    );


    // --------------------------------------------------------
    // 실제 화면 출력 크기
    // --------------------------------------------------------

    const float scale =
        drawSize /
        static_cast<float>(FRAME_H);


    sprite.setScale(
        {
            scale,
            scale
        }
    );


    // --------------------------------------------------------
    // 플레이어 위치
    // --------------------------------------------------------

    sprite.setPosition(
        getRenderPos(*this)
    );


    window.draw(sprite);
}


// ============================================================
// Player 업데이트
// ============================================================

void updatePlayer(
    Player& p,
    const TileMap& map,
    float dt
)
{
    // --------------------------------------------------------
    // 죽었거나 갇혀있으면 이동 금지
    // --------------------------------------------------------

    if (!p.alive || p.trapped)
    {
        p.moving = false;

        p.currentFrame = 0;
        p.animTimer = 0.f;

        return;
    }


    // ========================================================
    // 현재 이동 중
    // ========================================================

    if (p.moving)
    {
        p.progress += p.speed * dt;


        // ----------------------------------------------------
        // 걷기 애니메이션
        // ----------------------------------------------------

        p.animTimer += dt;

        if (p.animTimer >= p.animSpeed)
        {
            p.animTimer = 0.f;

            p.currentFrame =
                (p.currentFrame + 1) % FRAME_COUNT;
        }


        // ----------------------------------------------------
        // 목표 타일 도착
        // ----------------------------------------------------

        if (p.progress >= 1.f)
        {
            p.row = p.targetRow;
            p.col = p.targetCol;

            p.moving = false;

            p.progress = 0.f;

            p.currentFrame = 0;
            p.animTimer = 0.f;
        }

        return;
    }


    // ========================================================
    // 새로운 입력
    // ========================================================

    int dx = 0;
    int dy = 0;


    // --------------------------------------------------------
    // 위
    // --------------------------------------------------------

    if (sf::Keyboard::isKeyPressed(p.keyUp))
    {
        dy = -1;

        p.dir = "up";
    }


    // --------------------------------------------------------
    // 아래
    // --------------------------------------------------------

    else if (sf::Keyboard::isKeyPressed(p.keyDown))
    {
        dy = 1;

        p.dir = "down";
    }


    // --------------------------------------------------------
    // 왼쪽
    // --------------------------------------------------------

    else if (sf::Keyboard::isKeyPressed(p.keyLeft))
    {
        dx = -1;

        p.dir = "left";
    }


    // --------------------------------------------------------
    // 오른쪽
    // --------------------------------------------------------

    else if (sf::Keyboard::isKeyPressed(p.keyRight))
    {
        dx = 1;

        p.dir = "right";
    }


    // ========================================================
    // 이동 입력이 들어온 경우
    // ========================================================

    if (dx != 0 || dy != 0)
    {
        const int nextCol =
            p.col + dx;

        const int nextRow =
            p.row + dy;


        // ----------------------------------------------------
        // A 담당 TileMap에게
        // 다음 칸 이동 가능 여부 확인
        // ----------------------------------------------------

        if (map.isWalkable(nextCol, nextRow))
        {
            p.targetCol = nextCol;
            p.targetRow = nextRow;

            p.moving = true;

            p.progress = 0.f;

            p.currentFrame = 0;
            p.animTimer = 0.f;
        }

        else
        {
            // 벽 / 블록
            p.moving = false;

            p.currentFrame = 0;
            p.animTimer = 0.f;
        }
    }

    else
    {
        p.currentFrame = 0;
        p.animTimer = 0.f;
    }
}


// ============================================================
// 실제 화면 좌표 계산
// ============================================================

sf::Vector2f getRenderPos(const Player& p)
{
    float renderRow;
    float renderCol;


    // --------------------------------------------------------
    // 이동 중
    // --------------------------------------------------------

    if (p.moving)
    {
        renderRow =
            static_cast<float>(p.row)
            +
            static_cast<float>(
                p.targetRow - p.row
                )
            * p.progress;


        renderCol =
            static_cast<float>(p.col)
            +
            static_cast<float>(
                p.targetCol - p.col
                )
            * p.progress;
    }

    // --------------------------------------------------------
    // 정지
    // --------------------------------------------------------

    else
    {
        renderRow =
            static_cast<float>(p.row);

        renderCol =
            static_cast<float>(p.col);
    }


    // ========================================================
    // A 담당 맵 좌표계
    //
    // TileSize = 40
    // OriginX = 0
    // OriginY = 40
    // ========================================================

    const float x =
        static_cast<float>(TileMap::OriginX)
        +
        renderCol *
        static_cast<float>(TileMap::TileSize)
        +
        static_cast<float>(TileMap::TileSize) / 2.f;


    const float y =
        static_cast<float>(TileMap::OriginY)
        +
        renderRow *
        static_cast<float>(TileMap::TileSize)
        +
        static_cast<float>(TileMap::TileSize) / 2.f;


    return { x, y };
}


// ============================================================
// 현재 타일 위치
// ============================================================

sf::Vector2i getTilePosition(const Player& p)
{
    return {
        p.col,
        p.row
    };
}


// ============================================================
// 사망 처리
// ============================================================

void killPlayer(Player& p)
{
    p.alive = false;

    p.moving = false;

    p.progress = 0.f;

    p.currentFrame = 0;

    p.animTimer = 0.f;
}


// ============================================================
// 생존 여부
// ============================================================

bool isAlive(const Player& p)
{
    return p.alive;
}