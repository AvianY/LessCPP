#ifndef MENU_CONTEXT_LESSCPP
#define MENU_CONTEXT_LESSCPP

#include "const_globals.h"
#include "helpers.h"
#include "less.h"
#include "ai.h"
#include <tuple>

#include "context.h"
#include "game_context.h"
#include "pager_context.h"

#include "menu.h"

#include <array>

class MenuContext : public Context {
private:
	// side text
	sf::Font font = getFont("resources/Roboto_Medium.ttf");
	sf::Text text = initializeText(font, 60, window_width/3, window_height/3, sf::Color::Green);

	menu::Button quitButton = menu::Button("Quit", window_width/2 - 15, window_height - 50, 30);
	menu::Button startGameButton = menu::Button("Start Game", window_width/3 + 50, window_height/3 + 120, 30);
	menu::Button howToPlayButton = menu::Button("How to play", window_width/3 + 40, window_height/3 + 200, 30);

public:

	MenuContext();

	Context* update(const sf::Event &,  const sf::Vector2f & /* Mouse position */) override;
	sf::Texture render(const sf::Vector2f &) override;

};


#endif // GAME_CONTEXT_LESSCPP

