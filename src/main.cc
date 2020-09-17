#include "const_globals.h"
#include "wall.h"
#include "helpers.h"
#include "game.h"
#include "gui.h"
#include "less.h"
#include "ai.h"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <algorithm>
#include <chrono>
#include <thread>
#include <set>

#include <iostream>
#include <deque>


int main() {

	sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Less game", sf::Style::Close);
	window.setPosition(sf::Vector2i(0, 0));
	window.setFramerateLimit(30);

	auto wall_configs = wall::generateNwallconfigs<9>();

	Game game = Game(wall_configs);
	Gui gui = Gui(wall_configs);

	Player * clicked_player = nullptr;

	// load game sounds
	std::deque<sf::SoundBuffer> soundBuffers;

	sf::Sound sound_pickup = getSound("sounds/sfx_menu_move2.wav", soundBuffers);
	sf::Sound sound_drop = getSound("sounds/sfx_menu_move3.wav", soundBuffers);
	sf::Sound sound_illegal = getSound("sounds/sfx_sounds_error10.wav", soundBuffers);

	// generate side text
	sf::Font font = getFont("resources/Roboto_Medium.ttf");
	sf::Text text = initializeSideText(font, window_height + 10, 10, sf::Color::Green);

	while (window.isOpen()) {
		text.setString(get_side_text(game));

		auto event = sf::Event();
		while (window.pollEvent(event)) {
			// ON CLOSE EVENT
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			// CLOSE WHEN PRESSED Q
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Q) {
					window.close();
				}
			}
			// HUMAN
			if ( game.getState() != State::ENDED and game.active_side() == Side::WHITE ) {
				// GRAB PLAYER
				if (event.type == sf::Event::MouseButtonPressed) {
					for (auto &player : gui.getPlayers(game.active_side())) {
						if (euclideanDistance(getMousePosition(window), player.getPosition()) <= player_size) {
							sound_pickup.play();
							clicked_player = &player;
						}
					}
				}
				// DROP PLAYER
				if (event.type == sf::Event::MouseButtonReleased) {
					if (clicked_player != nullptr) {
						std::optional<Location> new_location = getMouseLocation(window);

						if (new_location){
							if (game.movePlayer(clicked_player->getLocation(), new_location.value())){
								clicked_player->setLocation(new_location.value());
								sound_drop.play();
							} else {
								sound_illegal.play();
							}
						} else {
							sound_illegal.play();
						}
						clicked_player->resetPosition();
						clicked_player = nullptr;
					}
				}
			}
			// AI
			else if ( game.getState() != State::ENDED and game.active_side() == Side::BLACK ) {
				auto path = recurseFindOptimal(game, Side::BLACK, 1, 0, 100, evaluation(game)).value();
				for( auto elem: path) {
					if (game.active_side() != Side::BLACK) break;
					auto newLocation = game.movePlayer(std::get<0>(elem), std::get<1>(elem));
					gui.getPlayers(Side::BLACK)[static_cast<uint>(std::get<0>(elem))].setLocation(newLocation.value());
					gui.getPlayers(Side::BLACK)[static_cast<uint>(std::get<0>(elem))].resetPosition();
					window.clear();
					window.draw(text);
					window.draw(gui);
					window.display();
				}
			}
		}


		// DRAWING
		window.clear();

		if (clicked_player != nullptr) clicked_player->setPosition(getMousePosition(window));

		window.draw(text);

		window.draw(gui);
		if (clicked_player != nullptr) window.draw(*clicked_player);

		window.display();



	}

	return 0;
}
