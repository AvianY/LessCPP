
#include "SFMLWidgets/button.h"

#include <iostream>

namespace widget
{

Button::Button(const std::string& text, const Settings& S)
    : FramedText(text, S)
{
}

bool Button::contains(sf::Vector2f pos)
{
    return m_frame.getGlobalBounds().contains(pos);
}

}
