#include "SFMLWidgets/list_view.h"

#include <algorithm>

namespace widget
{

ListView::ListView(const std::vector<std::string>& list, int width, int height, const Settings& S)
    : m_frame(sf::Vector2f(width, height))
    , m_vertical_spacing(S.chSize / 5)
    , m_displayed_lines(std::min<int>(height / (S.chSize + m_vertical_spacing), list.size()))
    , m_scrollbar(sf::Vector2f(S.chPixelWidth / 2, height * (m_displayed_lines / static_cast<float>(list.size())) ))
    , m_chSize(S.chSize)
    , m_chPixelWidth(S.chPixelWidth)
{
    m_frame.setFillColor(sf::Color::Transparent);
    m_frame.setOutlineThickness(-static_cast<int>(S.chSize) / 10);

    m_scrollbar.setFillColor(sf::Color::White);

    // set size of each line in m_lines
    int max_characters = width / S.chPixelWidth - 1; // subtract one width for m_frames width
    for (auto elem : list) {
        elem.resize(max_characters, ' ');
        m_lines.emplace_back(std::move(elem), S.font, S.chSize);
    }

    // set position to origin
    setPosition(0, 0);
}

void ListView::setPosition(uint x, uint y)
{
    m_frame.setPosition(x, y);
    for (auto i = 0; i <= m_lines.size(); ++i) {
        m_lines[i].setPosition(x + m_chPixelWidth / 2, y + (i - m_scroll_idx) * (m_vertical_spacing + m_chSize));
    }
    m_scrollbar.setPosition(x + m_frame.getGlobalBounds().width - m_scrollbar.getGlobalBounds().width,
                            y + m_scroll_idx * m_frame.getGlobalBounds().height / (m_lines.size() - m_displayed_lines));
    // m_scrollbar.setPosition(100, 100);
}

void ListView::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    for (auto i = m_scroll_idx; i < m_scroll_idx + m_displayed_lines; ++i) {
        target.draw(m_lines[i], states);
    }
    target.draw(m_frame, states);
    target.draw(m_scrollbar, states);
}

bool ListView::contains(sf::Vector2f pos)
{
    return m_frame.getGlobalBounds().contains(pos);
}

void ListView::scroll(int mousewheel_delta)
{
    int new_scroll_idx = std::clamp<int>(m_scroll_idx - mousewheel_delta, 0, m_lines.size() - m_displayed_lines);
    for (auto &line : m_lines) {
        line.move(0, -(new_scroll_idx - m_scroll_idx) * (m_vertical_spacing + m_chSize));
    }
    m_scrollbar.move(0, (new_scroll_idx - m_scroll_idx) * (m_frame.getGlobalBounds().height - m_scrollbar.getGlobalBounds().height) / static_cast<float>(m_lines.size() - m_displayed_lines));
    m_scroll_idx = new_scroll_idx;
}

}