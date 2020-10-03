#include "pager_context.h"

PagerContext::PagerContext(const std::string & title, const std::string & content ) {
	rentex.create(window_width, window_height);
	
	titleText.setString(title);
	contentText.setString(content);
}

Context* PagerContext::update(const sf::Event & event, const sf::Vector2f & mouse_pos)
{
	if (event.type == sf::Event::MouseButtonPressed) {
		if (returnButton.contains(mouse_pos)) quit = true;
	}
	if (event.type == sf::Event::KeyPressed) {
	}
	return nullptr;
}

sf::Texture PagerContext::render(const sf::Vector2f & mouse_pos) {

	rentex.clear();

	rentex.draw(titleText);
	rentex.draw(contentText);
	rentex.draw(returnButton);

	rentex.display();

	return rentex.getTexture();
}


