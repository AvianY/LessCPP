#ifndef FRAMED_TEXT_H
#define FRAMED_TEXT_H

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Export.hpp>
#include <SFML/Graphics/Shape.hpp>

#include "SFMLWidgets/settings.h"


// helper for menu creation

namespace widget
{

class FramedText : public sf::Drawable
{
protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
    sf::RectangleShape m_frame;
    sf::Text m_text;
    uint m_chPixelWidth;

public:
    FramedText(std::string text, const Settings& S);
    FramedText() = delete;

    void setPosition(uint x, uint y);
    void setTextFillColor(const sf::Color&);
};

}

#endif // FRAMED_TEXT_H
