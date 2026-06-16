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

	[[nodiscard]] bool IsEmpty() const { return cards.empty(); }

	void Draw(bool &isSelected);
};
