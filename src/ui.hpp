#pragma once
#include "pile.hpp"
#include "types.hpp"

class Ui {
  public:
	Ui() = default;
	~Ui() = default;

	static void Draw(const int &hp, const int &score,
					 const int &cardsRemaining);
	static void DrawHoveredCardInfo(const Card *card);
};
