#ifndef WALL_H
#define WALL_H

#include <array>
#include <random>
#include <algorithm>
#include "const_globals.h"

namespace wall {

	// One "WallConfig" relates to 2x2 block on board
	// A wall config has up to 3 wall segments
	// Wall segment is short blue line of length 1. (not 2!)
	// A wall segment is encoded by 3 numbers:
	// {first, second, third} <-- wall segment
	// first  number denotes -1 (horizontal), 1 (vertical), -2 (no wall/not used)
	// second number denotes -1 (left/up), 0 (middle), 1 (right/down)
	// third  number denotes -1 (left/up) or 1 (right/down)

		constexpr WallConfig bFullSide = {
						{
										{1, -1, -1},
										{1, -1, 1},
										{-2, 0, 0}
						}
		};
		constexpr WallConfig bHalfSide = {
						{
										{1, -1, 1},
										{-2, 0, 0},
										{-2, 0, 0}

						}

		};
		constexpr WallConfig bZigZag = {
						{
										{1, -1, -1},
										{-1, 0, -1},
										{1, 0, 1}
						}
		};
		constexpr WallConfig bT_Block = {
						{
										{1, -1, -1},
										{1, -1, 1},
										{-1, 0, -1}
						}
		};
		constexpr WallConfig bTopLeft = {
						{
										{1, -1, -1},
										{-1, 0, -1},
										{-2, 0, 0}

						}
		};
		constexpr WallConfig bTopRight = {
						{
										{1, 0, -1},
										{-1, 0, 1},
										{-2, 0, 0}
						}
		};
		constexpr WallConfig bBottomRight = {
						{
										{1, 0, 1},
										{-1, 1, 1},
										{-2, 0, 0}
						}
		};


		constexpr std::array<WallConfig, 7> unique_wall_configs{
						bFullSide, bHalfSide, bZigZag, bT_Block, bTopLeft, bTopRight, bBottomRight};

        WallConfig rotateWallConfig(WallConfig wall, uint rotation);

        template<size_t N>
        std::array<WallConfig, N> generateNwallconfigs() {
            auto eng = std::default_random_engine(std::random_device()());
            std::uniform_int_distribution<unsigned int> rand0to6(0, 6);
            std::uniform_int_distribution<unsigned int> rand0to3(0, 3);

            std::array<WallConfig, N> wallconfig;
            for (size_t i = 0; i != N; ++i) {
				wallconfig[i] = wall::rotateWallConfig(unique_wall_configs[rand0to6(eng)], rand0to3(eng));
            }
            return wallconfig;
        }

		constexpr bool hasWallSeg(const WallConfig &config, const WallSeg &seg) {
			for (auto const& w: config) {
				if (w[0] == seg[0] and w[1] == seg[1] and w[2] == seg[2]) return true;
			}
			return false;
//			return std::find(config.begin(), config.end(), seg) != config.end();
		}


} // namespace wall

#endif /* WALL_H */
