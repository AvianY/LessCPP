#ifndef FIELD_H
#define FIELD_H

#include "const_globals.h"
#include <random>
#include <array>
#include "block_shape.h"
#include "player.h"

#include "helpers.h"
#include <random>
#include <cassert>

#include <iostream>

struct Field {
private:
    int X = 3;
    int Y = 3;

    sf::RenderWindow & r_window;

    std::vector<std::vector<Block>> field;

    std::array<Player, 4> whitePlayers;
    std::array<Player, 4> blackPlayers;

    Player * selected_player = nullptr;

    int m_moves_left = 3;
    bool m_game_over = false;
    sf::Color m_active_side = White;
    sf::Color m_winning_side;
    int m_white_moves = 0;
    int m_black_moves = 0;

public:
    Field(sf::RenderWindow & window) : r_window(window) {

        auto eng = std::default_random_engine(std::random_device()());
        std::uniform_int_distribution<uint8_t> rand0to6(0, 6);
        std::uniform_int_distribution<int8_t> rand0to3(0, 3);

        // generate field blocks
        for (int y = 0; y < Y; ++y) {
            std::vector<Block> tmp;
            for (int x = 0; x < X; ++x) {
                Block tmp_block{block_size, block_border, wall::wall_configs[rand0to6(eng)], rand0to3(eng)};
                tmp_block.setPosition(x * block_size + block_size / 2, y * block_size + block_size / 2);
                tmp.emplace_back(tmp_block);
            }
            field.emplace_back(tmp);
        }

        // generate players
        for( size_t i = 0; i < whitePlayers.size(); ++i ) {
            whitePlayers[i]= (Player(player_size, White, whiteStart[i]));
        }

        for( size_t i = 0; i < blackPlayers.size(); ++i ) {
            blackPlayers[i]= (Player(player_size, Black, blackStart[i]));
        }
    }

    [[nodiscard]] int white_moves() const { return m_white_moves; }
    [[nodiscard]] int black_moves() const { return m_black_moves; }
    [[nodiscard]] sf::Color active_side() const { return m_active_side; }
    [[nodiscard]] std::array<Player, 4> & active_players() { return m_active_side == White ? this->whitePlayers : this->blackPlayers; }
    [[nodiscard]] sf::Color winning_side() const {return m_winning_side;}
    [[nodiscard]] int moves_left() const { return m_moves_left; }
    [[nodiscard]] bool game_over() const { return m_game_over; }

    bool move_piece(int cost) {
        if ( cost == 0 || cost > m_moves_left ) return false;
        (m_active_side == White ? m_white_moves : m_black_moves) += cost;
        m_moves_left -= cost;
        if ( m_moves_left == 0 ) {
            next_turn();
            m_moves_left = 3;
        }
        return true;
    }

    void next_turn() { m_active_side = (m_active_side == White ? Black : White); }

    void setGameOver(sf::Color winner) { m_game_over = true; m_winning_side = winner; }

    int countInnerWalls(Location const start, Location const end) const {

        // differences
        int const dx = end[0] - start[0];
        int const dy = end[1] - start[1];

        // block coordinates
        int const sx = start[0] / 2;
        int const sy = start[1] / 2;

        int const ex = end[0] / 2;
        int const ey = end[1] / 2;
        Block const & bstart = this->field[sy][sx];
        Block const & bend = this->field[ey][ex];

        // blocks are orthogonal and at most one space apart
        assert((dx == 0 && abs(dy) == 1) || (abs(dx) == 1 && dy == 0));

        int const first = (dx ? 1 : -1);
        int const second = (sx == ex && sy == ey ? 0 : dx + dy);
        int const third = -1 + 2 * abs(dx) * (start[1] % 2) + 2 * abs(dy) * (start[0] % 2);

        int num = 0;
        if (second) {
            num += bend.hasWall({first, -second, third});
        }
        num += bstart.hasWall({first, second, third});

        return num;
    }

    bool isPlayerAtLocation(Location const location) const {
        bool toReturn = false;
        for (const auto &player : whitePlayers) {
            if (player.getLocation() == location) toReturn = true;
        }
        for (const auto &player : blackPlayers) {
            if (player.getLocation() == location) toReturn = true;
        }
        return toReturn;
    }

    // return 0 means that it's impossible to make that move
    int moveCost(Location oldL, Location newL) const {
        if (newL[0] - oldL[0] != 0 && newL[1] - oldL[1] != 0) return 0;

        if (isPlayerAtLocation(newL)) return 0;

        int wallCount = 0;
        int distance = abs(newL[0] - oldL[0]) + abs(newL[1] - oldL[1]);
        if (distance > 2) return 0;
        if (distance == 2) {
            Location const connecting = {(newL[0] + oldL[0]) / 2, (newL[1] + oldL[1]) / 2};
            wallCount += countInnerWalls(oldL, connecting);
            wallCount += countInnerWalls(connecting, newL);

            if (!isPlayerAtLocation(connecting)) return 0;
            if (wallCount != 0) return 0;
            return 1;
        } else {
            wallCount += countInnerWalls(oldL, newL);
            return wallCount + 1;
        }


        return 0;
    }

    bool playersInLocations(std::array<Player, 4> const &players,
                            std::array<Location, 4> const &locations) const {
        for ( auto & player: players) {
            if(std::find(locations.begin(), locations.end(), player.getLocation()) == locations.end()) {
                return false;
            }
        }
        return true;
    }

    void selectPlayer(Player & player) {
        player.setSelected();
        this->selected_player = &player;
    }

    void unselectPlayer() {
        if (existsPlayerSelected()) {
            this->selected_player->setPosition(this->selected_player->getLocation());
            this->selected_player->unsetSelected();
            this->selected_player = nullptr;
        }
    }

    bool existsPlayerSelected() const {
        return this->selected_player==nullptr ? false : true;
    }

    Location selectedPlayerLocation() {
        return this->selected_player->getLocation();
    }

    void moveSelectedPlayer(Location new_location){
        Location old_location = this->selected_player->getLocation();
        if (new_location == old_location) return;
        int cost = moveCost(old_location, new_location);

        // move player to new location if move_piece was successful
        this->selected_player->setLocation(move_piece(cost) ? new_location : old_location);
        unselectPlayer();

        if (playersInLocations(whitePlayers, blackStart)) setGameOver(White);
        if (playersInLocations(blackPlayers, whiteStart)) setGameOver(Black);
    }

    void draw() {
        for (auto &row: field) {
            for (auto &block: row) {
                r_window.draw(block);
            }
        }

        for (auto &players: {whitePlayers, blackPlayers}){
            for(auto &player: players) {
                if(player.isSelected()) continue;
                r_window.draw(player);
            }
        }

        // Separately draw selected_player last so it will be drawn on top of others
        if (existsPlayerSelected()) {
            this->selected_player->setPosition(getMousePosition(r_window));
            r_window.draw(*this->selected_player);
        }
    }

};

#endif // FIELD_H

