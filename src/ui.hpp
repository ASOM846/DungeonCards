#pragma once
#include "pile.hpp"
#include "types.hpp"
#include <raylib.h>

class Ui {
  public:
	Ui() = default;
	~Ui() = default;

	static void Draw(const int &hp, const int &score,
					 const int &cardsRemaining);

	static void DrawMessageBox(const Card *card);
	static void DrawMessageRect(const Rectangle &dst, const char *text);
};
