#include "const_globals.h"
#include "game.h"
#include "helpers.h"
#include "wall.h"

#include <array>
#include <stdexcept>
#include <algorithm>

Game::Game(std::array<WallConfig, 9> wallconfigs) {
    for (size_t y = 0; y < 3; ++y) {
        for (size_t x = 0; x < 3; ++x) {
            m_wall_matrix[y][x] = wallconfigs[y*3 + x];
        }
    }
}

bool Game::operator==(const Game & game) const {
    if ( playersInLocations(m_whiteLocations, game.m_whiteLocations) and
         playersInLocations(m_blackLocations, game.m_blackLocations) ) return true;
    return false;
}

// Getter methods
uint Game::white_moves() const { return m_white_moves; }
uint Game::black_moves() const { return m_black_moves; }
State Game::getState() const { return m_state; }
Side Game::active_side() const { return m_active_side; }
Side Game::winning_side() const {return m_winning_side;}
uint Game::moves_left() const { return m_moves_left; }

std::array<Location, 4> & Game::active_players() {
    return m_active_side == Side::WHITE? m_whiteLocations : m_blackLocations;
}

bool Game::isEndOfTurn() const {
    return m_moves_left == 0 or (active_side() == Side::WHITE
                                 ? playersInLocations(m_whiteLocations, blackStart)
                                 : playersInLocations(m_blackLocations, whiteStart));
}


void Game::nextTurn() {
    m_active_side = (m_active_side == Side::WHITE ? Side::BLACK : Side::WHITE);

    if ( playersInLocations(m_blackLocations, whiteStart) or m_state == State::LAST_TURN ) m_state = State::ENDED;
    else if ( playersInLocations(m_whiteLocations, blackStart) ) { m_state = State::LAST_TURN; m_moves_left = 3 - m_moves_left; }
    else { m_moves_left = 3; }

    if ( m_state == State::ENDED ) {
        if ( playersInLocations(m_blackLocations, whiteStart) and playersInLocations(m_whiteLocations, blackStart) ) m_winning_side = Side::NONE;
        else if (playersInLocations(m_whiteLocations, blackStart)) m_winning_side = Side::WHITE;
        else m_winning_side = Side::BLACK;
    }
}

void Game::setGameOver(Side winner) { m_state = State::ENDED; m_winning_side = winner; }

constexpr uint Game::countInnerWalls(Location const start, Location const end) const {

    // differences
    int const dx = static_cast<int>(end.x - start.x);
    int const dy = static_cast<int>(end.y - start.y);

    // block coordinates
    uint const sx = uint(start.x / 2);
    uint const sy = uint(start.y / 2);

    uint const ex = uint(end.x / 2);
    uint const ey = uint(end.y / 2);
    WallConfig const & wstart = m_wall_matrix[sy][sx];
    WallConfig const & wend = m_wall_matrix[ey][ex];

    // blocks are orthogonal and at most one space apart
//    assert((dx == 0 && abs(dy) == 1) || (abs(dx) == 1 && dy == 0));

    int const first = (dx ? 1 : -1);
    int const second = (sx == ex && sy == ey ? 0 : dx + dy);
    int const third = -1 + 2 * cabs(dx) * static_cast<int>(start.y % 2) + 2 * cabs(dy) * static_cast<int>(start.x % 2);

    uint num = 0;
    if (second) {
        num += wall::hasWallSeg(wend, {first, -second, third});
    }
    num += wall::hasWallSeg(wstart, {first, second, third});

    return num;
}

constexpr bool Game::existsPlayerAtLocation(Location const location) const {
    bool toReturn = false;
    for (const auto &player : m_whiteLocations) {
        if (player == location) toReturn = true;
    }
    for (const auto &player : m_blackLocations) {
        if (player == location) toReturn = true;
    }
    return toReturn;
}

constexpr std::optional<uint> Game::moveCost(Location old_location, Direction direction) const {
    if (std::optional<Location> new_location = old_location + direction) {
        return countInnerWalls(old_location, new_location.value());
    }
    return {};
}

constexpr std::optional<uint> Game::moveCost(Location oldL, Location newL) const {

    if (oldL == newL) return 0;

    if (newL.x - oldL.x != 0 && newL.y - oldL.y != 0) return {};

    if (existsPlayerAtLocation(newL)) return {};

    int wallCount = 0;
    int distance = abs(static_cast<int>(newL.x - oldL.x)) + abs(static_cast<int>(newL.y - oldL.y));
    if (distance > 2) return {};
    if (distance == 2) {
        Location const connecting = {(newL.x + oldL.x) / 2, (newL.y + oldL.y) / 2};
        wallCount += countInnerWalls(oldL, connecting);
        wallCount += countInnerWalls(connecting, newL);

        if (!existsPlayerAtLocation(connecting)) return {};
        if (wallCount != 0) return {};
        return 1;
    }
    wallCount += countInnerWalls(oldL, newL);
    return wallCount + 1;
}


std::optional<uint> Game::getPlayerNumber(Location location) {
    for (size_t i = 0; i != 4; ++i) {
        if( active_players()[i] == location ) return i;
    }
    return {};
}

Location Game::getPlayerLocation(uint player_num) {
    return active_players()[player_num];
}

bool Game::decrementMoves(uint moves) {
    if (m_moves_left < moves ) return false;
    (m_active_side == Side::WHITE ? m_white_moves : m_black_moves) += moves;
    m_moves_left -= moves;
    return true;
}

std::optional<Location> Game::movePlayer(uint player_num, Direction direction) {
    Location old_location = getPlayerLocation(player_num);
    std::optional<uint> cost;
    std::optional<Location> new_location = old_location + direction;

    if (new_location){
        if ( existsPlayerAtLocation(new_location.value()) ) {
            if ((new_location = new_location.value() + direction)) {
                cost = moveCost(old_location, new_location.value());
            }
        } else {
            cost = moveCost(old_location, new_location.value());
        }
    }

    if ( cost && decrementMoves(cost.value())) {
        setPlayerLocation(player_num, new_location.value());
    } else {
        return {};
    }

    if ( isEndOfTurn() ) nextTurn();
    return (new_location && cost)? new_location: std::nullopt;
}

std::optional<Location> Game::movePlayer(Location old_location, Location new_location){
    auto player = getPlayerNumber(old_location);
    if (!player) return {};
    std::optional<uint> cost = moveCost(old_location, new_location);

    if (cost && decrementMoves(cost.value())){
        // the moves have been 'paid' successfully, we now have to move to player
        setPlayerLocation(player.value(), new_location);
    } else {
        return {};
    }

    if ( isEndOfTurn() ) nextTurn();

    return new_location;
}

void Game::setPlayerLocation(uint player, Location location) {
    active_players()[player] = location;
}

