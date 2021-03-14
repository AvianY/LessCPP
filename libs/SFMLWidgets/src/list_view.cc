#include "SFMLWidgets/list_view.h"


namespace widget
{

ListView::ListView(std::vector<std::string> list, size_t width, size_t height, const Settings& S)
    : m_list(std::move(list))
    , m_frame(sf::RectangleShape({static_cast<float>(width), static_cast<float>(height)}))
    , m_width(width)
    , m_height(height)
    , m_chPixelWidth(S.chPixelWidth)
    , m_chSize(S.chSize)
{
    m_frame.setFillColor(sf::Color::Transparent);
    m_frame.setOutlineThickness(-static_cast<int>(S.chSize) / 10);

    // set size of each line in m_lines
    size_t max_lines = height / (S.chSize + S.chSize / 5 /* line spacing */);
    size_t iterations = m_list.size() > max_lines ? max_lines : m_list.size();
    size_t max_characters = m_width / m_chPixelWidth - 1; // subtract one width for m_frames width
    for (size_t i = 0; i < iterations; ++i) {
        m_list[i].resize(max_characters, ' ');
        m_lines.emplace_back(m_list[i], S.font, S.chSize);
    }

    // set position to origin
    setPosition(0, 0);
}

void ListView::setPosition(uint x, uint y)
{
    m_frame.setPosition(x, y);
    for (size_t i = 0; i < m_lines.size(); ++i) {
        m_lines[i].setPosition(x + m_chPixelWidth / 2, y + i * m_chSize +  i * m_chSize / 5 /* line spacing */);
    }
}

void ListView::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    for (const auto& line: m_lines) {
        target.draw(line, states);
    }

    target.draw(m_frame, states);
}

}