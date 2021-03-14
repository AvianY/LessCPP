#ifndef MENU_BUTTON_H
#define MENU_BUTTON_H

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Export.hpp>
#include <SFML/Graphics/Shape.hpp>

#include "SFMLWidgets/settings.h"
#include "SFMLWidgets/framed_text.h"
#include <string>


// helper for menu creation

namespace widget
{

class Button : public FramedText
{
public:
    Button(const std::string& text, const Settings& S);
    Button() = delete;

    bool contains(sf::Vector2f pos);
};

}

#endif // MENU_BUTTON_H
