#include "ui.hpp"
#include <raylib.h>

void Ui::Draw(const int &hp, const int &score, const int &cardsRemaining) {
	DrawText(TextFormat("HP: %d", hp), 40, GetScreenHeight() - 40, 20, RED);

	DrawText(TextFormat("SCORE: %d", score), 120, GetScreenHeight() - 40, 20,
			 BLUE);

	DrawText(TextFormat("CARDS REMAINING: %d", cardsRemaining), 240,
			 GetScreenHeight() - 40, 20, VIOLET);
}

void Ui::DrawHoveredCardInfo(const Card *card) {
	if (card == nullptr) {
		return;
	}

	const char *cardDsc = card->description.c_str();

	int fontSize = 20;

	int width = MeasureText(cardDsc, fontSize);
	TraceLog(LOG_INFO, "DRAWED INFO=========================");
	DrawText(cardDsc, GetScreenWidth() - width - 50, GetScreenHeight() - 40,
			 fontSize, RED);
}

void Ui::DrawCardBackground(Rectangle &dst, bool isCard) {}
