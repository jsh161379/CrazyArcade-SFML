#include "Player.h"
#include "TileMap.h"

#include <SFML/Graphics.hpp>

#include <iostream>
#include <optional>
#include <string>
#include <vector>


// ============================================================
// Scene
// ============================================================

enum class Scene
{
    SELECT,
    GAME
};


// ============================================================
// UTF-8 문자열
// ============================================================

sf::String toKor(const std::string& text)
{
    return sf::String::fromUtf8(
        text.begin(),
        text.end()
    );
}


// ============================================================
// Text 중앙 정렬
// ============================================================

void centerTextX(
    sf::Text& text,
    float centerX
)
{
    const sf::FloatRect bounds =
        text.getLocalBounds();

    text.setPosition(
        {
            centerX
                - (bounds.position.x + bounds.size.x / 2.f),

            text.getPosition().y
        }
    );
}


// ============================================================
// 캐릭터 카드 그리기
// ============================================================

void drawCharacterCard(
    sf::RenderWindow& window,
    const sf::Texture& texture,
    int index,
    int selectedP1,
    int selectedP2,
    bool p1Confirmed,
    bool p2Confirmed,
    float centerX,
    float centerY
)
{
    const bool isP1 =
        index == selectedP1;

    const bool isP2 =
        index == selectedP2;


    // ========================================================
    // 선택된 카드는 조금 크게
    // ========================================================

    float cardWidth = 86.f;
    float cardHeight = 116.f;

    if (isP1 || isP2)
    {
        cardWidth = 94.f;
        cardHeight = 124.f;
    }


    // ========================================================
    // 그림자
    // ========================================================

    sf::RectangleShape shadow(
        {
            cardWidth,
            cardHeight
        }
    );

    shadow.setOrigin(
        {
            cardWidth / 2.f,
            cardHeight / 2.f
        }
    );

    shadow.setPosition(
        {
            centerX + 4.f,
            centerY + 6.f
        }
    );

    shadow.setFillColor(
        sf::Color(
            8,
            9,
            20,
            150
        )
    );

    window.draw(shadow);


    // ========================================================
    // 카드 본체
    // ========================================================

    sf::RectangleShape card(
        {
            cardWidth,
            cardHeight
        }
    );

    card.setOrigin(
        {
            cardWidth / 2.f,
            cardHeight / 2.f
        }
    );

    card.setPosition(
        {
            centerX,
            centerY
        }
    );


    // 선택되지 않은 카드
    card.setFillColor(
        sf::Color(
            42,
            45,
            69
        )
    );

    card.setOutlineThickness(2.f);

    card.setOutlineColor(
        sf::Color(
            75,
            80,
            110
        )
    );


    // P1 선택
    if (isP1 && !isP2)
    {
        card.setFillColor(
            sf::Color(
                67,
                38,
                51
            )
        );

        card.setOutlineThickness(4.f);

        card.setOutlineColor(
            p1Confirmed
            ? sf::Color(90, 230, 130)
            : sf::Color(255, 75, 90)
        );
    }


    // P2 선택
    if (isP2 && !isP1)
    {
        card.setFillColor(
            sf::Color(
                35,
                49,
                76
            )
        );

        card.setOutlineThickness(4.f);

        card.setOutlineColor(
            p2Confirmed
            ? sf::Color(90, 230, 130)
            : sf::Color(70, 150, 255)
        );
    }


    // P1 / P2 둘 다 같은 캐릭터 선택
    if (isP1 && isP2)
    {
        card.setFillColor(
            sf::Color(
                60,
                42,
                75
            )
        );

        card.setOutlineThickness(5.f);

        card.setOutlineColor(
            sf::Color(
                205,
                100,
                255
            )
        );
    }


    window.draw(card);


    // ========================================================
    // 캐릭터 이미지
    // ========================================================

    sf::Sprite sprite(texture);

    sprite.setTextureRect(
        sf::IntRect(
            {
                0,
                0
            },
            {
                FRAME_W,
                FRAME_H
            }
        )
    );

    sprite.setOrigin(
        {
            FRAME_W / 2.f,
            FRAME_H / 2.f
        }
    );


    const float imageSize =
        (isP1 || isP2)
        ? 69.f
        : 62.f;


    const float scale =
        imageSize /
        static_cast<float>(
            FRAME_H
            );


    sprite.setScale(
        {
            scale,
            scale
        }
    );

    sprite.setPosition(
        {
            centerX,
            centerY - 4.f
        }
    );


    window.draw(sprite);


    // ========================================================
    // 카드 아래 번호 바
    // ========================================================

    sf::RectangleShape numberBar(
        {
            cardWidth - 10.f,
            20.f
        }
    );

    numberBar.setOrigin(
        {
            (cardWidth - 10.f) / 2.f,
            10.f
        }
    );

    numberBar.setPosition(
        {
            centerX,
            centerY + cardHeight / 2.f - 16.f
        }
    );

    numberBar.setFillColor(
        sf::Color(
            20,
            22,
            37,
            210
        )
    );

    window.draw(numberBar);
}


// ============================================================
// 선택 포인터 P1/P2
// ============================================================

void drawPlayerMarker(
    sf::RenderWindow& window,
    sf::Font& font,
    const std::string& text,
    float x,
    float y,
    const sf::Color& color
)
{
    sf::RectangleShape marker(
        {
            38.f,
            22.f
        }
    );

    marker.setOrigin(
        {
            19.f,
            11.f
        }
    );

    marker.setPosition(
        {
            x,
            y
        }
    );

    marker.setFillColor(color);

    window.draw(marker);


    sf::Text markerText(
        font,
        text,
        13
    );

    markerText.setStyle(
        sf::Text::Bold
    );

    markerText.setFillColor(
        sf::Color::White
    );


    const sf::FloatRect bounds =
        markerText.getLocalBounds();

    markerText.setPosition(
        {
            x
                - bounds.position.x
                - bounds.size.x / 2.f,

            y
                - bounds.position.y
                - bounds.size.y / 2.f
                - 1.f
        }
    );

    window.draw(markerText);
}


// ============================================================
// main
// ============================================================

int main()
{
    // ========================================================
    // Window
    // ========================================================

    sf::RenderWindow window(
        sf::VideoMode(
            {
                800u,
                600u
            }
        ),
        "Crazy Arcade"
    );

    window.setFramerateLimit(60);


    // ========================================================
    // Character textures
    // ========================================================

    std::vector<std::string> characterPaths =
    {
        "assets/characters/char1.png",
        "assets/characters/char2.png",
        "assets/characters/char3.png",
        "assets/characters/char4.png",
        "assets/characters/char5.png",
        "assets/characters/char6.png"
    };


    std::vector<sf::Texture>
        characterTextures;


    for (
        const std::string& path :
        characterPaths
        )
    {
        sf::Texture texture;

        if (!texture.loadFromFile(path))
        {
            std::cout
                << path
                << " 로드 실패"
                << std::endl;

            return -1;
        }

        characterTextures.push_back(
            std::move(texture)
        );
    }


    // ========================================================
    // Font
    // ========================================================

    sf::Font font;

    if (
        !font.openFromFile(
            "assets/font.ttf"
        )
        )
    {
        std::cout
            << "font 로드 실패"
            << std::endl;

        return -1;
    }


    // ========================================================
    // Map
    // ========================================================

    TileMap map;


    // ========================================================
    // Players
    // ========================================================

    Player p1{
        1,
        1,
        1,
        1
    };


    Player p2{
        TileMap::Rows - 2,
        TileMap::Columns - 2,

        TileMap::Rows - 2,
        TileMap::Columns - 2
    };


    // P1 WASD
    p1.keyUp =
        sf::Keyboard::Scancode::W;

    p1.keyDown =
        sf::Keyboard::Scancode::S;

    p1.keyLeft =
        sf::Keyboard::Scancode::A;

    p1.keyRight =
        sf::Keyboard::Scancode::D;


    // P2 Arrow
    p2.keyUp =
        sf::Keyboard::Scancode::Up;

    p2.keyDown =
        sf::Keyboard::Scancode::Down;

    p2.keyLeft =
        sf::Keyboard::Scancode::Left;

    p2.keyRight =
        sf::Keyboard::Scancode::Right;


    // ========================================================
    // Select State
    // ========================================================

    Scene scene =
        Scene::SELECT;


    int selectP1 = 0;
    int selectP2 = 1;

    bool p1Confirmed = false;
    bool p2Confirmed = false;


    // ========================================================
    // UI Layout
    // ========================================================

    const float cardStartX =
        135.f;

    const float cardGap =
        106.f;

    const float cardY =
        285.f;


    // ========================================================
    // Clock
    // ========================================================

    sf::Clock clock;


    // ========================================================
    // Game Loop
    // ========================================================

    while (window.isOpen())
    {
        // ====================================================
        // Events
        // ====================================================

        while (
            const std::optional event =
            window.pollEvent()
            )
        {
            if (
                event->is<
                sf::Event::Closed
                >()
                )
            {
                window.close();
            }


            // =================================================
            // SELECT
            // =================================================

            if (scene == Scene::SELECT)
            {
                if (
                    const auto* key =
                    event->getIf<
                    sf::Event::KeyPressed
                    >()
                    )
                {
                    const int count =
                        static_cast<int>(
                            characterPaths.size()
                            );


                    // =========================================
                    // P1
                    // =========================================

                    if (!p1Confirmed)
                    {
                        if (
                            key->scancode ==
                            sf::Keyboard::Scancode::A
                            )
                        {
                            selectP1--;

                            if (selectP1 < 0)
                                selectP1 =
                                count - 1;
                        }


                        else if (
                            key->scancode ==
                            sf::Keyboard::Scancode::D
                            )
                        {
                            selectP1++;

                            if (
                                selectP1 >= count
                                )
                            {
                                selectP1 = 0;
                            }
                        }


                        else if (
                            key->scancode ==
                            sf::Keyboard::Scancode::Space
                            )
                        {
                            p1Confirmed = true;
                        }
                    }


                    // =========================================
                    // P2
                    // =========================================

                    if (!p2Confirmed)
                    {
                        if (
                            key->scancode ==
                            sf::Keyboard::Scancode::Left
                            )
                        {
                            selectP2--;

                            if (selectP2 < 0)
                                selectP2 =
                                count - 1;
                        }


                        else if (
                            key->scancode ==
                            sf::Keyboard::Scancode::Right
                            )
                        {
                            selectP2++;

                            if (
                                selectP2 >= count
                                )
                            {
                                selectP2 = 0;
                            }
                        }


                        else if (
                            key->scancode ==
                            sf::Keyboard::Scancode::Enter
                            )
                        {
                            p2Confirmed = true;
                        }
                    }


                    // =========================================
                    // 취소 기능
                    //
                    // P1 = Q
                    // P2 = RShift
                    // =========================================

                    if (
                        key->scancode ==
                        sf::Keyboard::Scancode::Q
                        )
                    {
                        p1Confirmed = false;
                    }


                    if (
                        key->scancode ==
                        sf::Keyboard::Scancode::RShift
                        )
                    {
                        p2Confirmed = false;
                    }


                    // =========================================
                    // 둘 다 선택 완료
                    // =========================================

                    if (
                        p1Confirmed &&
                        p2Confirmed
                        )
                    {
                        if (
                            !p1.loadTexture(
                                characterPaths[
                                    selectP1
                                ]
                            )
                            )
                        {
                            return -1;
                        }


                        if (
                            !p2.loadTexture(
                                characterPaths[
                                    selectP2
                                ]
                            )
                            )
                        {
                            return -1;
                        }


                        scene =
                            Scene::GAME;

                        clock.restart();
                    }
                }
            }


            // =================================================
            // GAME
            // =================================================

            else
            {
                if (
                    const auto* key =
                    event->getIf<
                    sf::Event::KeyPressed
                    >()
                    )
                {
                    if (
                        key->scancode ==
                        sf::Keyboard::Scancode::Escape
                        )
                    {
                        window.close();
                    }
                }
            }
        }


        // ====================================================
        // Delta time
        // ====================================================

        float dt =
            clock.restart().asSeconds();


        if (dt > 0.1f)
        {
            dt = 0.1f;
        }


        // ====================================================
        // UPDATE
        // ====================================================

        if (scene == Scene::GAME)
        {
            updatePlayer(
                p1,
                map,
                dt
            );

            updatePlayer(
                p2,
                map,
                dt
            );
        }


        // ====================================================
        // BACKGROUND
        // ====================================================

        window.clear(
            sf::Color(
                19,
                21,
                38
            )
        );


        // ====================================================
        // SELECT SCREEN
        // ====================================================

        if (scene == Scene::SELECT)
        {
            // =================================================
            // 상단 장식
            // =================================================

            sf::RectangleShape topBar(
                {
                    800.f,
                    7.f
                }
            );

            topBar.setFillColor(
                sf::Color(
                    86,
                    112,
                    255
                )
            );

            window.draw(topBar);


            // =================================================
            // Title
            // =================================================

            sf::Text logo(
                font,
                "CRAZY ARCADE",
                34
            );

            logo.setStyle(
                sf::Text::Bold
            );

            logo.setFillColor(
                sf::Color(
                    245,
                    247,
                    255
                )
            );

            logo.setPosition(
                {
                    0.f,
                    42.f
                }
            );

            centerTextX(
                logo,
                400.f
            );

            window.draw(logo);


            sf::Text subtitle(
                font,
                toKor(
                    "플레이할 캐릭터를 선택하세요"
                ),
                18
            );

            subtitle.setFillColor(
                sf::Color(
                    165,
                    171,
                    200
                )
            );

            subtitle.setPosition(
                {
                    0.f,
                    91.f
                }
            );

            centerTextX(
                subtitle,
                400.f
            );

            window.draw(subtitle);


            // =================================================
            // 중앙 캐릭터 영역 패널
            // =================================================

            sf::RectangleShape characterPanel(
                {
                    700.f,
                    184.f
                }
            );

            characterPanel.setPosition(
                {
                    50.f,
                    186.f
                }
            );

            characterPanel.setFillColor(
                sf::Color(
                    28,
                    31,
                    52
                )
            );

            characterPanel.setOutlineThickness(
                1.f
            );

            characterPanel.setOutlineColor(
                sf::Color(
                    60,
                    65,
                    95
                )
            );

            window.draw(
                characterPanel
            );


            // =================================================
            // 캐릭터 카드
            // =================================================

            for (
                int i = 0;
                i <
                static_cast<int>(
                    characterPaths.size()
                    );
                ++i
                )
            {
                const float x =
                    cardStartX
                    +
                    static_cast<float>(i)
                    * cardGap;


                drawCharacterCard(
                    window,
                    characterTextures[i],
                    i,
                    selectP1,
                    selectP2,
                    p1Confirmed,
                    p2Confirmed,
                    x,
                    cardY
                );


                // =============================================
                // P1 Marker
                // =============================================

                if (i == selectP1)
                {
                    drawPlayerMarker(
                        window,
                        font,
                        p1Confirmed
                        ? "P1 OK"
                        : "P1",
                        x - 20.f,
                        205.f,
                        p1Confirmed
                        ? sf::Color(
                            55,
                            190,
                            105
                        )
                        : sf::Color(
                            230,
                            65,
                            80
                        )
                    );
                }


                // =============================================
                // P2 Marker
                // =============================================

                if (i == selectP2)
                {
                    drawPlayerMarker(
                        window,
                        font,
                        p2Confirmed
                        ? "P2 OK"
                        : "P2",
                        x + 20.f,
                        205.f,
                        p2Confirmed
                        ? sf::Color(
                            55,
                            190,
                            105
                        )
                        : sf::Color(
                            60,
                            125,
                            235
                        )
                    );
                }
            }


            // =================================================
            // 하단 P1 패널
            // =================================================

            sf::RectangleShape p1Panel(
                {
                    325.f,
                    115.f
                }
            );

            p1Panel.setPosition(
                {
                    55.f,
                    410.f
                }
            );

            p1Panel.setFillColor(
                sf::Color(
                    48,
                    29,
                    40
                )
            );

            p1Panel.setOutlineThickness(
                2.f
            );

            p1Panel.setOutlineColor(
                p1Confirmed
                ? sf::Color(
                    65,
                    210,
                    115
                )
                : sf::Color(
                    220,
                    60,
                    75
                )
            );

            window.draw(p1Panel);


            // =================================================
            // P1 Text
            // =================================================

            sf::Text p1Title(
                font,
                p1Confirmed
                ? "P1  READY!"
                : "PLAYER 1",
                19
            );

            p1Title.setStyle(
                sf::Text::Bold
            );

            p1Title.setFillColor(
                p1Confirmed
                ? sf::Color(
                    90,
                    235,
                    135
                )
                : sf::Color(
                    255,
                    90,
                    100
                )
            );

            p1Title.setPosition(
                {
                    77.f,
                    427.f
                }
            );

            window.draw(p1Title);


            sf::Text p1Control(
                font,
                toKor(
                    "A / D  캐릭터 선택"
                ),
                15
            );

            p1Control.setFillColor(
                sf::Color(
                    220,
                    220,
                    232
                )
            );

            p1Control.setPosition(
                {
                    77.f,
                    460.f
                }
            );

            window.draw(
                p1Control
            );


            sf::Text p1Confirm(
                font,
                toKor(
                    "SPACE  선택 완료"
                ),
                15
            );

            p1Confirm.setFillColor(
                sf::Color(
                    165,
                    167,
                    190
                )
            );

            p1Confirm.setPosition(
                {
                    77.f,
                    487.f
                }
            );

            window.draw(
                p1Confirm
            );


            // =================================================
            // 하단 P2 패널
            // =================================================

            sf::RectangleShape p2Panel(
                {
                    325.f,
                    115.f
                }
            );

            p2Panel.setPosition(
                {
                    420.f,
                    410.f
                }
            );

            p2Panel.setFillColor(
                sf::Color(
                    27,
                    38,
                    61
                )
            );

            p2Panel.setOutlineThickness(
                2.f
            );

            p2Panel.setOutlineColor(
                p2Confirmed
                ? sf::Color(
                    65,
                    210,
                    115
                )
                : sf::Color(
                    65,
                    135,
                    245
                )
            );

            window.draw(p2Panel);


            // =================================================
            // P2 Text
            // =================================================

            sf::Text p2Title(
                font,
                p2Confirmed
                ? "P2  READY!"
                : "PLAYER 2",
                19
            );

            p2Title.setStyle(
                sf::Text::Bold
            );

            p2Title.setFillColor(
                p2Confirmed
                ? sf::Color(
                    90,
                    235,
                    135
                )
                : sf::Color(
                    80,
                    155,
                    255
                )
            );

            p2Title.setPosition(
                {
                    442.f,
                    427.f
                }
            );

            window.draw(p2Title);


            sf::Text p2Control(
                font,
                toKor(
                    "← / →  캐릭터 선택"
                ),
                15
            );

            p2Control.setFillColor(
                sf::Color(
                    220,
                    220,
                    232
                )
            );

            p2Control.setPosition(
                {
                    442.f,
                    460.f
                }
            );

            window.draw(
                p2Control
            );


            sf::Text p2Confirm(
                font,
                toKor(
                    "ENTER  선택 완료"
                ),
                15
            );

            p2Confirm.setFillColor(
                sf::Color(
                    165,
                    167,
                    190
                )
            );

            p2Confirm.setPosition(
                {
                    442.f,
                    487.f
                }
            );

            window.draw(
                p2Confirm
            );


            // =================================================
            // 하단 메시지
            // =================================================

            sf::Text bottomText(
                font,
                toKor(
                    "두 플레이어가 모두 선택하면 게임이 시작됩니다"
                ),
                13
            );

            bottomText.setFillColor(
                sf::Color(
                    115,
                    120,
                    150
                )
            );

            bottomText.setPosition(
                {
                    0.f,
                    557.f
                }
            );

            centerTextX(
                bottomText,
                400.f
            );

            window.draw(
                bottomText
            );
        }


        // ====================================================
        // GAME
        // ====================================================

        else if (
            scene == Scene::GAME
            )
        {
            map.draw(window);

            p1.draw(window);

            p2.draw(window);
        }


        window.display();
    }


    return 0;
}