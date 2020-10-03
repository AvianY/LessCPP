#ifndef PAGER_CONTEXT_LESSCPP
#define PAGER_CONTEXT_LESSCPP

#include "const_globals.h"

#include "context.h"
#include "menu.h"
#include <string>

class PagerContext : public Context {
private:
	// side text
	sf::Font font = getFont("resources/Roboto_Medium.ttf");
	sf::Text titleText = initializeText(font, 60, window_width/3, 0, sf::Color::Green);
	sf::Text contentText = initializeText(font, 15, 20, 100, sf::Color::White);

	menu::Button returnButton = menu::Button("Return", window_width/2 - 15, window_height - 50, 30);
	
	std::vector<std::string> splitted_text;

public:

	PagerContext(const std::string &, const std::string &);

	Context* update(const sf::Event &,  const sf::Vector2f & /* Mouse position */) override;
	sf::Texture render(const sf::Vector2f &) override;

};


#endif // PAGER_CONTEXT_LESSCPP


