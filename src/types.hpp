#pragma once

#include <raylib.h>
#include <string>
#include <vector>

enum class CardType {
	ENEMY,
	WEAPON,
	POTION,
	PLAYER,
	COUNT
};

enum PlayerSlot {
	P_LEFT,
	P_PLAYER,
	P_RIGHT,
	P_BACKPACK,
	P_COUNT
};

enum DungeonSlot {
	D_ONE,
	D_TWO,
	D_THREE,
	D_COUNT
};

struct Card {
	std::string name;
	int value;
	CardType type;
};
