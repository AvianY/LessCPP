#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Export.hpp>
#include <SFML/Graphics/Shape.hpp>

#include "const_globals.h"
#include "less.h"


// helper for menu creation

namespace menu
{
	 


class Button : public sf::Drawable
{
private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    sf::RectangleShape m_frame;
    sf::RectangleShape m_leftBanner;
    sf::RectangleShape m_rightBanner;

    sf::Text m_text;
	sf::Font m_font;


public:
    Button(std::string text, int xpos, int ypos, int fontSize);
    Button() = delete;

	bool contains(sf::Vector2f pos) { return m_frame.getGlobalBounds().contains(pos); }

};



}




#endif // MENU_H
