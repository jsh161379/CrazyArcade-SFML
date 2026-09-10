#include "UI.h"
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <sstream>
using namespace std;

bool UI::loadFont(const string& path)
{
    m_ready = m_font.openFromFile(path);
    return m_ready;
}
void UI::draw(sf::RenderTarget& target, const array<PlayerHudData, 2>& players,
              float secondsRemaining, const string& status) const
{
    if (!m_ready) return;
    const auto oldView = target.getView();
    target.setView(target.getDefaultView());
    const float width = target.getDefaultView().getSize().x;
    sf::RectangleShape panel({width, 114.f});
    panel.setFillColor(sf::Color(20, 29, 45, 235));
    target.draw(panel);
    for (size_t i = 0; i < players.size(); ++i)
    {
        const auto& p = players[i];
        ostringstream line;
        line << (i + 1) << "P  " << (!p.alive ? "OUT" : p.trapped ? "TRAPPED" : "READY")
             << "\nBomb " << p.stats.bombCapacity << "  Range " << p.stats.blastRange
             << "\nSpeed " << fixed << setprecision(0) << p.stats.speed << "  Needle " << p.stats.needles;
        sf::Text text(m_font, line.str(), 17);
        text.setPosition({12.f + static_cast<float>(i) * width / 2.f, 8.f});
        text.setFillColor(i == 0 ? sf::Color(110, 220, 255) : sf::Color(255, 205, 100));
        target.draw(text);
    }
    const int time = isfinite(secondsRemaining) ? static_cast<int>(clamp(secondsRemaining, 0.f, 359999.f)) : 0;
    ostringstream label;
    label << "TIME " << time / 60 << ':' << setw(2) << setfill('0') << time % 60;
    if (!status.empty()) label << "  " << status;
    const string message = label.str();
    sf::Text text(m_font, sf::String::fromUtf8(message.begin(), message.end()), 16);
    text.setPosition({12.f, 87.f});
    target.draw(text);
    target.setView(oldView);
}
