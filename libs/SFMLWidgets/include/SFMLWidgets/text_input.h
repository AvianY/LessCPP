#ifndef MENU_TEXT_INPUT_H
#define MENU_TEXT_INPUT_H

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Export.hpp>
#include <SFML/Graphics/Shape.hpp>

#include "settings.h"
#include <string>
#include <iso646.h>

// helper for menu creation

namespace widget
{
     

class TextInput : public sf::Drawable
{
private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::Text m_nameText;
    const size_t m_maxLength;

    sf::Text m_insertText;
    std::string m_input_string;
    sf::RectangleShape m_frame;

    size_t m_cursor;
    sf::RectangleShape m_cursorLine;

    Validator m_validator;

    uint m_chPixelWidth;
    uint m_chSize;

    bool focused = false;


public:
    void validate();
    void backspace();
    void delete_front();
    void append(char c);
    std::string getText() {return m_input_string; }
    void setPosition(uint xpos, uint ypos);
    void cursorLeft();
    void cursorRight();

    void processInputEvent(const sf::Event& inputEvent);

    TextInput& operator()(TextInput*);


    TextInput(std::string nameText,
              uint charWidth,
              const Settings& S,
              std::string defaultText = "",
              std::function<bool(std::string)> V = [](...){return true;}
    );
    TextInput() = delete;

    bool contains(sf::Vector2f pos) { return m_frame.getGlobalBounds().contains(pos); }

};

}

#endif // MENU_TEXT_INPUT_H
