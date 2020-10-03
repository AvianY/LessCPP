#include "menu_context.h"

MenuContext::MenuContext() {
	rentex.create(window_width, window_height);
}

Context* MenuContext::update(const sf::Event & event, const sf::Vector2f & mouse_pos)
{
	if (event.type == sf::Event::MouseButtonPressed) {
		if (quitButton.contains(mouse_pos)) quit = true;
		else if (startGameButton.contains(mouse_pos)) return new GameContext();
		else if (howToPlayButton.contains(mouse_pos)) return new PagerContext("How to play", "Take those pieces and move them around lol. Don't ask me how to play this haha.");
	}
	if (event.type == sf::Event::KeyPressed) {
	}
	return nullptr;
}

sf::Texture MenuContext::render(const sf::Vector2f & mouse_pos) {
	text.setString("Less Game");

	rentex.clear();

	rentex.draw(text);
	rentex.draw(quitButton);
	rentex.draw(howToPlayButton);
	rentex.draw(startGameButton);
	

	rentex.display();

	return rentex.getTexture();
}

