#pragma once

#include "textureManager.hpp"
#include <algorithm>
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
	NONE,
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

	int minValue = -1;
	int maxValue = -1;

	int hp;

	int durability;
	int maxDurability;

	CardType type;
	Element element;

	TextureId textureId;

	[[nodiscard]] int GetRandomVal() const {
		return (GetRandomValue(minValue, maxValue));
	}

	void Clear() {
		name = "---";
		description = " --- - ---";
		minValue = -1;
		maxValue = -1;
		hp = -1;
		durability = -1;
		maxDurability = -1;
		type = CardType::NONE;
		element = Element::NONE;
		textureId = TextureId::NONE;
	}

	void IncreaseHp(int val) {
		hp += val;

		if (maxValue == -1) {
			return;
		}

		hp = std::min(hp, maxValue);
	}
};
