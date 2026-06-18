#pragma once

#include "textureManager.hpp"
#include <raylib.h>
#include <string>

enum class Element {
	NONE,
	FIRE,
	ICE,
	LIFESTEAL,
	WARHAMMER, // back to master deck
	ESCAPE,	   // redraw 4 cards
	SACRIFICE, // deal damage equal to points remaining to max heal
	COUNT
};

enum class CardType {
	ENEMY,
	WEAPON,
	SPELL,
	WAND,
	SHIELD,
	POTION,
	PLAYER,
	COIN,
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
	D_FOUR,
	D_COUNT
};

struct Card {
	std::string name;
	std::string description;
	int value;
	int maxValue = -1;
	CardType type;
	Element element;

	TextureId textureId;

	void IncreaseVal(int val) {
		value += val;

		if (maxValue == -1) {
			return;
		}

		if (value >= maxValue) {
			value = maxValue;
		}
	}
};
