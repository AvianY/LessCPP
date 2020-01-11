#include "const_globals.h"
#include "wall.h"
#include "helpers.h"
#include "game.h"
#include "gui.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <algorithm>
#include <chrono>
#include <thread>
#include <set>
#include <utility>

#include <iostream>

using namespace std::string_literals;

constexpr int sumLocation(Location loc) {
	return loc.x + loc.y;
}

constexpr int evaluation(const Game & game) {
	const auto whites = game.getPlayers(Side::WHITE);
	const auto blacks = game.getPlayers(Side::BLACK);
	int eval = 0;
	for (size_t i = 0; i < 4; ++i) {
		eval += sumLocation(blacks[i] + whites[i]);
	}
	if (playersInLocations(whites, blackStart)) eval += 4;
	else if (playersInLocations(blacks, whiteStart)) eval -= 4;
	return eval;
}

//static std::vector<std::pair<std::set<Location>, std::set<Location>>> games;

// path = player, direction, evaluation
typedef std::tuple<int, Direction, int> Path;
std::optional<std::vector<Path>> recurseFindOptimal(const Game state, const Side side, int depth, int alpha, int beta, int lasteval) {
	std::vector<std::vector<Path>> paths;
//	std::vector<int> evals;

	for (int player = 0; player < 4; ++player) {
		Direction direction = Direction::UP;
		do {
			if (alpha >= beta ) goto SKIPALL;
			Game newstate = state;
			if(auto location = newstate.movePlayer(player, direction)){
				auto const neweval = evaluation(newstate);
				if (newstate.active_side() == side) {
					if (auto aux = recurseFindOptimal(newstate, side, depth, alpha, beta, lasteval)) {
						aux.value().emplace(aux.value().begin(), std::make_tuple(player, direction, neweval));
						paths.push_back(aux.value());
					}
				} else if (depth > 0 and state.getState() != State::LAST_TURN) {
//					if (evals.size() == 0) evals.emplace_back(neweval);
//					else if (side == Side::BLACK and evals[0] < neweval) evals.emplace(evals.begin(), neweval);
//					else if (evals.back() > neweval) evals.emplace_back(neweval);
//					else continue;
					if (side == Side::BLACK and neweval > lasteval - 2) continue;
					else if (side == Side::WHITE and neweval < lasteval + 2) continue;

					if (auto aux = recurseFindOptimal(newstate, ~side, depth - 1, alpha, beta, neweval)) {
						auto const prune = std::get<2>(aux.value().back());
						if (side == Side::WHITE) beta = std::min(beta, prune);
						else alpha = std::max(alpha, prune);
						aux.value().emplace(aux.value().begin(), std::make_tuple(player, direction, neweval));
						paths.push_back(aux.value());
					}
				} else {
					paths.emplace_back(std::vector<Path>{std::make_tuple(player, direction, neweval)});
				}
			}
		} while (++direction != Direction::UP);
	}
SKIPALL:
	if (paths.empty()) return std::nullopt;
	if (side == Side::BLACK)
		return *std::min_element(paths.begin(), paths.end(), [](std::vector<Path> a, std::vector<Path> b){ return std::get<2>(a.back()) < std::get<2>(b.back());});
	else
		return *std::max_element(paths.begin(), paths.end(), [](std::vector<Path> a, std::vector<Path> b){ return std::get<2>(a.back()) < std::get<2>(b.back());});
}


int main() {

	sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Less game", sf::Style::Close);
	window.setPosition(sf::Vector2i(0, 0));
	window.setFramerateLimit(30);

	auto wall_configs = wall::generateNwallconfigs<9>();

	Game game = Game(wall_configs);
	Gui gui = Gui(wall_configs);
	Player * clicked_player = nullptr;

	// load game sounds
	sf::SoundBuffer buffer_pickup;
	if (!buffer_pickup.loadFromFile("../sounds/sfx_menu_move2.wav"))
		throw std::runtime_error("Cannot find the sound file '../sounds/sfx_menu_move2.wav'");

	sf::SoundBuffer buffer_drop;
	if (!buffer_drop.loadFromFile("../sounds/sfx_menu_move3.wav"))
		throw std::runtime_error("Cannot find the sound file '../sounds/sfx_menu_move3.wav'");

	sf::SoundBuffer buffer_illegal;
	if (!buffer_illegal.loadFromFile("../sounds/sfx_sounds_error10.wav"))
		throw std::runtime_error("Cannot find the sound file '../sounds/sfx_sounds_error10.wav'");

	sf::Sound sound_drop, sound_pickup, sound_illegal;
	sound_drop.setBuffer(buffer_drop);
	sound_pickup.setBuffer(buffer_pickup);
	sound_illegal.setBuffer(buffer_illegal);

	// generate side text
	sf::Text text;
	sf::Font font;
	if (!font.loadFromFile("../resources/Roboto_Medium.ttf")) {
		if (!font.loadFromFile("resources/Roboto_Medium.ttf")) {
			throw std::runtime_error("Cannot find the font file 'resources/Roboto_Medium.ttf'");
		}
	}
	text.setFont(font);
	text.setCharacterSize(10);
	text.setPosition(window_height + 10, 10);
	text.setFillColor(sf::Color::Green);

	while (window.isOpen()) {

		std::string displayText;
		if ( game.getState() != State::ENDED ) {
			displayText += "Remaining moves: "s + std::to_string(game.moves_left()) + "\n"s;
			displayText += "Turn: "s + (game.active_side() == Side::WHITE ? "White"s : "Black"s) + "\n"s;
			displayText += "White total moves : "s + std::to_string(game.white_moves()) + "\n"s;
			displayText += "Black total moves : "s + std::to_string(game.black_moves()) + "\n"s;
		} else {
			displayText += "GAME OVER\n";
			if (game.winning_side() == Side::NONE){
				displayText += "It's a tie!"s;
			} else {
				displayText += (game.winning_side() == Side::WHITE ? "White"s : "Black"s) + " wins"s;
			}
		}
		text.setString(displayText);

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
			else if ( game.getState() != State::ENDED and game.active_side() == Side::BLACK ) {
				auto path = recurseFindOptimal(game, Side::BLACK, 2, 0, 100, evaluation(game)).value();
				for( auto elem: path) {
					if (game.active_side() != Side::BLACK) break;
					std::this_thread::sleep_for(std::chrono::seconds(1));
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
