#ifndef LIST_VIEW_H
#define LIST_VIEW_H

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Export.hpp>
#include <SFML/Graphics/Shape.hpp>

#include "SFMLWidgets/settings.h"

#include <string>
#include <vector>
#include <iso646.h>



namespace widget
{

class ListView : public sf::Drawable
{
private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    std::vector<std::string> m_list;

    sf::RectangleShape m_frame;
    std::vector<sf::Text> m_lines;

    size_t m_scroll_line = 0;
    size_t m_width;
    size_t m_height;

    size_t m_chPixelWidth;
    size_t m_chSize;

public:
    ListView() = delete;

    ListView(std::vector<std::string> list, size_t width, size_t height, const Settings& S);

    void setPosition(uint x, uint y);
};

}

#endif // LIST_VIEW_H
