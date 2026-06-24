#pragma once

#include "types.hpp"
#include <raylib.h>
#include <vector>

struct Pile {
	Pile() : position({.x = 0, .y = 0}) {};

	Pile(Vector2 position, int width, int height)
		: position{position}, width{width}, height{height} {};

	std::vector<Card> cards;
	Vector2 position;
	int width;
	int height;

	bool isDiscardPile = false;
	bool isLeftHand = false;
	bool isRightHand = false;
	bool isBackpack = false;

	bool isDungeonPile = false;

	[[nodiscard]] bool IsEmpty() const { return cards.empty(); }
	[[nodiscard]] Card &Back() { return cards.back(); }

	[[nodiscard]] Vector2 GetPosition() const { return position; }
	[[nodiscard]] Vector2 GetFloatingTextPos() const {
		return {position.x + width / 2, position.y + height / 3};
	}

	void Draw(TextureManager &tm, bool isSelected, bool isHighlited = false);

	void DrawCardBackground(Vector2 position);

	void DrawEnemy(const int &centerX, const int &centerY, Color color);
};
