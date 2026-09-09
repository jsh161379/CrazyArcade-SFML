#include <SFML/Graphics.hpp>

int main()
{
    // 800x600 크기의 윈도우 생성
    sf::RenderWindow window(
        sf::VideoMode({ 800, 600 }),
        "SFML Test"
    );

    // 게임 루프
    while (window.isOpen())
    {
        // 발생한 이벤트가 있는 동안 반복
        while (const std::optional event = window.pollEvent())
        {
            // 창의 X 버튼을 눌렀다면
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        // 화면을 파란색으로 지움
        window.clear(sf::Color::Blue);

        // 화면에 표시
        window.display();
    }

    return 0;
}