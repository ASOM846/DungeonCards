#include "ui.hpp"
#include <raylib.h>

void Ui::Draw(const int &hp, const int &score, const int &cardsRemaining) {
	DrawText(TextFormat("HP: %d", hp), 40, GetScreenHeight() - 40, 20, RED);

	DrawText(TextFormat("SCORE: %d", score), 120, GetScreenHeight() - 40, 20,
			 BLUE);

	DrawText(TextFormat("CARDS REMAINING: %d", cardsRemaining), 240,
			 GetScreenHeight() - 40, 20, VIOLET);
}

void Ui::DrawMessageBox(const Card *card) {
	if (card == nullptr) {
		return;
	}

	const char *text = card->description.c_str();
	float fontSize = 20.0f;
	float fontSpacing = 1.0f;
	Font font = GetFontDefault();

	Vector2 textSize = MeasureTextEx(font, text, fontSize, fontSpacing);

	float paddingX = 24.0f;
	float paddingY = 20.0f;

	Rectangle dst;
	dst.width = textSize.x + (paddingX * 2);
	dst.height = textSize.y + (paddingY * 2);

	dst.x = GetScreenWidth() - dst.width - 20;
	dst.y = GetScreenHeight() - dst.height - 20;

	DrawMessageRect(dst, text);
}

void Ui::DrawMessageRect(const Rectangle &dst, const char *text) {
	const auto shadow = Color{0, 0, 0, 100};
	const auto darkWood = Color{60, 30, 15, 255};
	const auto parchment = Color{220, 200, 170, 255};
	const auto goldAccent = Color{180, 130, 40, 255};

	DrawRectangle(dst.x + 4, dst.y + 4, dst.width, dst.height, shadow);
	DrawRectangle(dst.x, dst.y, dst.width, dst.height, darkWood);

	const float border = 6;
	DrawRectangle(dst.x + border, dst.y + border, dst.width - (2 * border),
				  dst.height - (2 * border), parchment);

	int innerMargin = border + 4;
	DrawRectangleLinesEx({dst.x + static_cast<float>(innerMargin),
						  dst.y + static_cast<float>(innerMargin),
						  static_cast<float>(dst.width - (2 * innerMargin)),
						  static_cast<float>(dst.height - (innerMargin * 2))},
						 2, goldAccent);

	float cornerLen = 16.0F;
	float thick = 4.0F;
	float p = innerMargin;

	Vector2 tl = {dst.x + p, dst.y + p};
	DrawLineEx(tl, {tl.x + cornerLen, tl.y}, thick, goldAccent);
	DrawLineEx(tl, {tl.x, tl.y + cornerLen}, thick, goldAccent);

	Vector2 tr = {dst.x + dst.width - p, dst.y + p};
	DrawLineEx(tr, {tr.x - cornerLen, tr.y}, thick, goldAccent);
	DrawLineEx(tr, {tr.x, tr.y + cornerLen}, thick, goldAccent);

	Vector2 bl = {dst.x + p, dst.y + dst.height - p};
	DrawLineEx(bl, {bl.x + cornerLen, bl.y}, thick, goldAccent);
	DrawLineEx(bl, {bl.x, bl.y - cornerLen}, thick, goldAccent);

	Vector2 br = {dst.x + dst.width - p, dst.y + dst.height - p};
	DrawLineEx(br, {br.x - cornerLen, br.y}, thick, goldAccent);
	DrawLineEx(br, {br.x, br.y - cornerLen}, thick, goldAccent);

	const float niteRadius = 3;
	DrawCircle(tl.x, tl.y, niteRadius, darkWood);
	DrawCircle(tr.x, tr.y, niteRadius, darkWood);
	DrawCircle(bl.x, bl.y, niteRadius, darkWood);
	DrawCircle(br.x, br.y, niteRadius, darkWood);

	float fontSize = 20.0f;
	float fontSpacing = 1.0f;
	Font font = GetFontDefault();

	Vector2 textSize = MeasureTextEx(font, text, fontSize, fontSpacing);

	float textX = (dst.x + dst.width / 2.0f) - (textSize.x / 2.0f);
	float textY = (dst.y + dst.height / 2.0f) - (textSize.y / 2.0f);

	DrawTextEx(font, text, {textX, textY}, fontSize, fontSpacing, darkWood);
}
