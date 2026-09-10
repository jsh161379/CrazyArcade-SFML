#include "Player.h"
#include <iostream>
#include <vector>

enum class Scene { SELECT, GAME };
sf::String toKor(const std::string& s) {
    return sf::String::fromUtf8(s.begin(), s.end());
}
int main() {
    sf::RenderWindow window(sf::VideoMode({ 800u, 600u }), "Water Balloon");

    // ---------- 선택 가능한 캐릭터 목록 ----------
    std::vector<std::string> characterPaths = {
        "assets/characters/char1.png",
        "assets/characters/char2.png",
        "assets/characters/char3.png",
        "assets/characters/char4.png",
        "assets/characters/char5.png",
        "assets/characters/char6.png"
    };

    std::vector<sf::Texture> charTextures;
    for (auto& path : characterPaths) {
        sf::Texture t;
        if (!t.loadFromFile(path)) {
            std::cout << path << " 로드 실패!\n";
            return -1;
        }
        charTextures.push_back(t);
    }

    sf::Font font;
    if (!font.openFromFile("assets/font.ttf")) {
        std::cout << "font.ttf 로드 실패!\n";
        return -1;
    }

    // ---------- 선택 화면 상태 ----------
    Scene scene = Scene::SELECT;
    int selIndexP1 = 0;
    int selIndexP2 = (int)characterPaths.size() > 1 ? 1 : 0;
    bool p1Confirmed = false;
    bool p2Confirmed = false;

    float cardSize = 80.f;
    float cardGap = 15.f;
    float startX = 660.f / 2.f - (characterPaths.size() * (cardSize + cardGap) - cardGap) / 2.f;
    float cardY = 220.f;

    sf::Text titleText(font, toKor("캐릭터를 선택하세요"), 24);
    titleText.setPosition({ 660.f / 2.f - 130.f, 40.f });
    titleText.setFillColor(sf::Color::White);

    sf::Text p1HintText(font, toKor("P1: A/D 이동  Space 확정"), 15);
    p1HintText.setPosition({ 40.f, 400.f });
    p1HintText.setFillColor(sf::Color::Red);

    sf::Text p2HintText(font, toKor("P2: Left/Right 이동  Enter 확정"), 15);
    p2HintText.setPosition({ 350.f, 400.f });
    p2HintText.setFillColor(sf::Color(80, 150, 255));

    // ---------- 게임 화면용 실제 캐릭터 (선택 확정 후 로드) ----------
    Player p1{ 2, 2, 2, 2 };
    p1.keyUp = sf::Keyboard::Scancode::W;
    p1.keyDown = sf::Keyboard::Scancode::S;
    p1.keyLeft = sf::Keyboard::Scancode::A;
    p1.keyRight = sf::Keyboard::Scancode::D;

    Player p2{ 7, 12, 7, 12 };
    p2.keyUp = sf::Keyboard::Scancode::Up;
    p2.keyDown = sf::Keyboard::Scancode::Down;
    p2.keyLeft = sf::Keyboard::Scancode::Left;
    p2.keyRight = sf::Keyboard::Scancode::Right;

    sf::Texture tex1, tex2;   // 선택 확정되면 여기 채워짐
    sf::Sprite sprite1(charTextures[0]); // 임시로 뭐든 넣어둠 (선택 전엔 안 그림)
    sf::Sprite sprite2(charTextures[0]);

    sf::Clock clock;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (scene == Scene::SELECT) {
                if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                    int count = (int)characterPaths.size();

                    // P1 조작
                    if (key->scancode == sf::Keyboard::Scancode::A && !p1Confirmed)
                        selIndexP1 = (selIndexP1 - 1 + count) % count;
                    if (key->scancode == sf::Keyboard::Scancode::D && !p1Confirmed)
                        selIndexP1 = (selIndexP1 + 1) % count;
                    if (key->scancode == sf::Keyboard::Scancode::Space)
                        p1Confirmed = true;

                    // P2 조작
                    if (key->scancode == sf::Keyboard::Scancode::Left && !p2Confirmed)
                        selIndexP2 = (selIndexP2 - 1 + count) % count;
                    if (key->scancode == sf::Keyboard::Scancode::Right && !p2Confirmed)
                        selIndexP2 = (selIndexP2 + 1) % count;
                    if (key->scancode == sf::Keyboard::Scancode::Enter)
                        p2Confirmed = true;

                    // 둘 다 확정하면 게임 화면으로 전환
                    if (p1Confirmed && p2Confirmed) {
                        tex1 = charTextures[selIndexP1];
                        tex2 = charTextures[selIndexP2];
                        sprite1 = sf::Sprite(tex1);
                        sprite2 = sf::Sprite(tex2);
                        sprite1.setOrigin({ FRAME_W / 2.f, FRAME_H / 2.f });
                        sprite2.setOrigin({ FRAME_W / 2.f, FRAME_H / 2.f });
                        float desiredSize = 36.f;
                        float scale = desiredSize / FRAME_H;
                        sprite1.setScale({ scale, scale });
                        sprite2.setScale({ scale, scale });

                        scene = Scene::GAME;
                    }
                }
            }
        }

        float dt = clock.restart().asSeconds();

        // ---------- 화면 그리기 ----------
        window.clear(sf::Color(30, 30, 50));

        if (scene == Scene::SELECT) {
            window.draw(titleText);
            window.draw(p1HintText);
            window.draw(p2HintText);

            for (int i = 0; i < (int)characterPaths.size(); i++) {
                sf::Sprite card(charTextures[i]);
                card.setTextureRect(sf::IntRect({ 0, 0 }, { FRAME_W, FRAME_H })); // 정면 정지 프레임만 미리보기
                card.setOrigin({ FRAME_W / 2.f, FRAME_H / 2.f });
                float scale = cardSize / FRAME_H;
                card.setScale({ scale, scale });

                float x = startX + i * (cardSize + cardGap) + cardSize / 2.f;
                card.setPosition({ x, cardY });
                window.draw(card);

                // 선택 표시 테두리
                sf::RectangleShape box({ cardSize + 8.f, cardSize + 8.f });
                box.setOrigin({ (cardSize + 8.f) / 2.f, (cardSize + 8.f) / 2.f });
                box.setPosition({ x, cardY });
                box.setFillColor(sf::Color::Transparent);

                if (i == selIndexP1) {
                    box.setOutlineColor(sf::Color::Red);
                    box.setOutlineThickness(3.f);
                    window.draw(box);
                }
                if (i == selIndexP2) {
                    sf::RectangleShape box2 = box;
                    box2.setSize({ cardSize + 16.f, cardSize + 16.f });
                    box2.setOrigin({ (cardSize + 16.f) / 2.f, (cardSize + 16.f) / 2.f });
                    box2.setOutlineColor(sf::Color(80, 150, 255));
                    box2.setOutlineThickness(3.f);
                    window.draw(box2);
                }
            }
        }
        else if (scene == Scene::GAME) {
            updatePlayer(p1, dt);
            updatePlayer(p2, dt);

            sprite1.setTextureRect(sf::IntRect(
                { p1.currentFrame * FRAME_W, dirRow(p1.dir) * FRAME_H },
                { FRAME_W, FRAME_H }
            ));
            sprite2.setTextureRect(sf::IntRect(
                { p2.currentFrame * FRAME_W, dirRow(p2.dir) * FRAME_H },
                { FRAME_W, FRAME_H }
            ));

            sprite1.setPosition(getRenderPos(p1));
            sprite2.setPosition(getRenderPos(p2));

            window.draw(sprite1);
            window.draw(sprite2);
        }

        window.display();
    }
    return 0;
}