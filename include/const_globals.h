//
// Created by aviany on 5/9/19.
//

#ifndef LESSCPP_CONST_GLOBALS_H
#define LESSCPP_CONST_GLOBALS_H

#include <utility>
#include <array>
#include <tuple>
#include <SFML/Graphics.hpp>

// typedef std::array<std::tuple<Orientation, Lane, >, 3> WallSeg1;


typedef std::array<int, 2> Location;
typedef std::array<int, 3> WallSeg;
typedef std::array<WallSeg, 3> WallConfig;

// individual color pieces starting positions
constexpr std::array<Location, 4> whiteStart = {{{0, 0},{1, 0},{0, 1},{1, 1}}};
constexpr std::array<Location, 4> blackStart = {{{4, 4},{5, 4},{4, 5},{5, 5}}};

constexpr float block_size = 200;
constexpr float block_border = 2;
constexpr float player_size = 30;

constexpr int window_width = 800;
constexpr int window_height = 600;

static sf::Color const White = {255, 222, 173};
static sf::Color const Black = {139, 69, 19};
static sf::Color const Noone = {0, 0, 0};

#endif //LESSCPP_CONST_GLOBALS_H
