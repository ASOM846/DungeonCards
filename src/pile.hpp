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

	[[nodiscard]] bool IsEmpty() const { return cards.empty(); }
	[[nodiscard]] Card &Back() { return cards.back(); }

	void Draw(bool &isSelected);

	void DrawEnemy(const int &centerX, const int &centerY, Color color);
};
