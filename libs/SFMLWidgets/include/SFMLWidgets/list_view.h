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

    sf::RectangleShape m_frame;

    int m_vertical_spacing;
    int m_displayed_lines;
    
    sf::RectangleShape m_scrollbar;
    std::vector<sf::Text> m_lines;

    int m_scroll_idx = 0; // iterator, that points to the first line to be displayed


    int m_chSize;
    int m_chPixelWidth;

public:
    ListView() = delete;

    ListView(const std::vector<std::string>& list, int width, int height, const Settings& S);

    void setPosition(uint x, uint y);

    bool contains(sf::Vector2f pos);

    void scroll(int mousewheel_delta);
};

}

#endif // LIST_VIEW_H
