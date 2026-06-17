#include "ui.hpp"
#include <raylib.h>

void Ui::Draw(const int &hp, const int &score, const int &cardsRemaining) {
	DrawText(TextFormat("HP: %d", hp), 40, GetScreenHeight() - 40, 20, RED);

	DrawText(TextFormat("SCORE: %d", score), 120, GetScreenHeight() - 40, 20,
			 BLUE);

	DrawText(TextFormat("CARDS REMAINING: %d", cardsRemaining), 240,
			 GetScreenHeight() - 40, 20, VIOLET);
}
