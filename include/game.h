#ifndef FIELD_H
#define FIELD_H

#include "const_globals.h"
#include <random>
#include <array>

#include "helpers.h"
#include <random>

#include <iostream>

class Game {
private:

//    Player * selected_player = nullptr;

	int m_moves_left = 3;
    State m_state = State::ONGOING;
    Side m_active_side = Side::WHITE;
    Side m_winning_side = Side::NONE;
	int m_white_moves = 0;
	int m_black_moves = 0;

    std::array<Location, 4> m_whiteLocations = whiteStart;
    std::array<Location, 4> m_blackLocations = blackStart;

    std::array<std::array<WallConfig, 3>, 3> m_wall_matrix;

	void setPlayerLocation(int, Location);

public:
    Game(std::array<WallConfig, 9>);
    Game(const Game &) = default;
    bool operator==(const Game &) const;

	[[nodiscard]] int white_moves() const;
	[[nodiscard]] int black_moves() const;
    [[nodiscard]] State getState() const;
    [[nodiscard]] Side active_side() const;
    [[nodiscard]] std::array<Location, 4> & active_players();
    [[nodiscard]] Side winning_side() const;
	[[nodiscard]] int moves_left() const;

	std::optional<int> getPlayerNumber(Location);

    constexpr std::array<Location, 4> getPlayers(Side side) const {
        if (side == Side::WHITE) {
            return m_whiteLocations;
        } else {
            return m_blackLocations;
        }
    }

    bool isEndOfTurn() const;

    void nextTurn();

    void setGameOver(Side);

	[[nodiscard]] constexpr int countInnerWalls(Location const, Location const) const;

	[[nodiscard]] constexpr bool existsPlayerAtLocation(Location const) const;

	[[nodiscard]] constexpr std::optional<int> moveCost (Location, Direction) const;

	[[nodiscard]] constexpr std::optional<int> moveCost (Location, Location) const;

	Location getPlayerLocation(int);

	[[nodiscard]] bool decrementMoves(int);

	std::optional<Location> movePlayer(int, Direction);

    std::optional<Location> movePlayer(Location, Location);

};

#endif // FIELD_H

