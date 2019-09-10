#include "const_globals.h"
#include "player.h"
#include "block_shape.h"
#include "helpers.h"
#include "field.h"
#include <SFML/Graphics.hpp>
#include <random>
#include <cassert>

using namespace std::string_literals;

//std::vector<std::vector<Block>> field;

//std::array<Player, 4> whitePlayers;
//std::array<Player, 4> blackPlayers;

//static Player * selected_player = nullptr;

//struct GameState {
//private:
//	int m_moves_left = 3;
//	bool m_game_over = false;
//	sf::Color m_active_side = White;
//	sf::Color m_winning_side;
//	int m_white_moves = 0;
//	int m_black_moves = 0;
//public:
//	GameState () = default;
//	[[nodiscard]] int white_moves() const { return m_white_moves; }
//	[[nodiscard]] int black_moves() const { return m_black_moves; }
//	[[nodiscard]] sf::Color active_side() const { return m_active_side; }
//	[[nodiscard]] sf::Color winning_side() const {return m_winning_side;}
//	[[nodiscard]] int moves_left() const { return m_moves_left; }
//	[[nodiscard]] bool game_over() const { return m_game_over; }


//	bool move_piece(int cost) {
//		if ( cost == 0 || cost > m_moves_left ) return false;
//		(m_active_side == White ? m_white_moves : m_black_moves) += cost;
//		m_moves_left -= cost;
//		if ( m_moves_left == 0 ) {
//			next_turn();
//			m_moves_left = 3;
//		}
//		return true;
//	}

//	void next_turn() { m_active_side = (m_active_side == White ? Black : White); }

//	void setGameOver(sf::Color winner) { m_game_over = true; m_winning_side = winner; }

//};

//int countInnerWalls(Location const start, Location const end) {

//	// differences
//	int const dx = end[0] - start[0];
//	int const dy = end[1] - start[1];

//	// block coordinates
//    int const sx = start[0] / 2;
//    int const sy = start[1] / 2;

//    int const ex = end[0] / 2;
//    int const ey = end[1] / 2;

//	Block bstart = field[sy][sx];
//	Block bend = field[ey][ex];

//	// blocks are orthogonal and at most one space apart
//	assert((dx == 0 && abs(dy) == 1) || (abs(dx) == 1 && dy == 0));

//	int const first = (dx ? 1 : -1);
//	int const second = (sx == ex && sy == ey ? 0 : dx + dy);
//	int const third = -1 + 2 * abs(dx) * (start[1] % 2) + 2 * abs(dy) * (start[0] % 2);

//	int N = 0;
//	if (second) {
//		N += bend.hasWall({first, -second, third});
//	}
//	N += bstart.hasWall({first, second, third});

//	return N;
//}

//bool isPlayerAtLocation(Location const location) {
//	bool toReturn = false;
//	for (const auto &player : whitePlayers) {
//		if (player.getLocation() == location) toReturn = true;
//	}
//	for (const auto &player : blackPlayers) {
//		if (player.getLocation() == location) toReturn = true;
//	}
//	return toReturn;
//}

//int moveCost(Location oldL, Location newL) {
//	if (newL[0] - oldL[0] != 0 && newL[1] - oldL[1] != 0) return 0;

//	if (isPlayerAtLocation(newL)) return 0;

//	int wallCount = 0;
//	int distance = abs(newL[0] - oldL[0]) + abs(newL[1] - oldL[1]);
//	if (distance > 2) return 0;
//	if (distance == 2) {
//		Location const connecting = {(newL[0] + oldL[0]) / 2, (newL[1] + oldL[1]) / 2};
//		wallCount += countInnerWalls(oldL, connecting);
//		wallCount += countInnerWalls(connecting, newL);

//		if (!isPlayerAtLocation(connecting)) return 0;
//		if (wallCount != 0) return 0;
//		return 1;
//	} else {
//		wallCount += countInnerWalls(oldL, newL);
//		return wallCount + 1;
//	}


//	return 0;
//}

//template <size_t N>
//bool playersInLocations(std::array<Player, N> const &players, std::array<Location, N> const &locations) {
//	for ( auto & player: players) {
//		if(std::find(locations.begin(), locations.end(), player.getLocation()) == locations.end()) {
//			return false;
//		}
//	}
//	return true;
//}

int main() {

	sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Less game", sf::Style::Close);

	// Game initialization
	std::default_random_engine eng((std::random_device()) ());
	std::uniform_int_distribution<int8_t> rand0to6(0, 6);
	std::uniform_int_distribution<int8_t> rand0to3(0, 3);

	window.setPosition(sf::Vector2i(0, 0));

    // create game field
    auto field = Field(window);

//	// generate the field
//	for (int y = 0; y < 3; ++y) {
//		std::vector<Block> tmp;
//		for (int x = 0; x < 3; ++x) {
//			Block tmp_block{block_size, block_border, wall::wall_configs[rand0to6(eng)], rand0to3(eng)};
//			tmp_block.setPosition(x * block_size + block_size / 2, y * block_size + block_size / 2);
//			tmp.emplace_back(tmp_block);
//		}
//		field.emplace_back(tmp);
//    }

//	// generate players
//	for( size_t i = 0; i < whitePlayers.size(); ++i ) {
//		whitePlayers[i]= (Player(player_size, White, whiteStart[i]));
//	}

//	for( size_t i = 0; i < blackPlayers.size(); ++i ) {
//		blackPlayers[i]= (Player(player_size, Black, blackStart[i]));
//	}

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
        if ( !field.game_over() ) {
            displayText += "Remaining moves: "s + std::to_string(field.moves_left()) + "\n"s;
            displayText += "Turn: "s + (field.active_side() == White ? "White"s : "Black"s) + "\n"s;
            displayText += "White total moves : "s + std::to_string(field.white_moves()) + "\n"s;
            displayText += "Black total moves : "s + std::to_string(field.black_moves()) + "\n"s;
		} else {
			displayText += "GAME OVER\n";
            displayText += (field.winning_side() == White ? "White"s : "Black"s) + " wins"s;
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
            if (!field.game_over()) {
				// GRAB PLAYER
				if (event.type == sf::Event::MouseButtonPressed) {
                    for (auto &player : field.active_players()) {
						if (euclideanDistance(getMousePosition(window), player.getPosition()) <= player_size) {
                            field.selectPlayer(player);
						}
					}
				}
				// DROP PLAYER
				if (event.type == sf::Event::MouseButtonReleased) {
                    if (field.existsPlayerSelected()) {
                        std::optional<Location> new_location = getMouseLocation(window);

                        if (new_location){
                            field.moveSelectedPlayer(new_location.value());
                        } else {
                            field.unselectPlayer();
                        }
					}

				}
			}
		}


		// DRAWING
		window.clear();

        field.draw();

		window.draw(text);

		window.display();

	}

	return 0;
}
