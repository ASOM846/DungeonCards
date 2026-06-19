#pragma once
#include "pile.hpp"
#include "types.hpp"
#include <raylib.h>

class Ui {
  public:
	Ui() = default;
	~Ui() = default;

	static void DrawBackground(const Texture2D &texture);

	static void DrawMessageBox(const Card *card, const int &score);
	static void DrawMessageRect(const Rectangle &dst, const char *text,
								float shadowOffset = 4.0f,
								const Font &font = GetFontDefault());

	static void DrawProgressBar(float maxVal, float currentVal);
	static void DrawTitle(const char *text, const Font &font);
};
