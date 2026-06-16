#pragma once

#include "pile.hpp"
struct GameState {
	Pile playerPiles[P_COUNT];
	Pile dungeonPiles[D_COUNT];
};
