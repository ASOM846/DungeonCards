#include "pile.hpp"

void Pile::Draw(bool &isSelected) {
	DrawRectangleV(position,
				   {static_cast<float>(width), static_cast<float>(height)},
				   GRAY);

	if (isSelected) {
		DrawRectangleLinesEx(
			{position.x, position.y, (float)width, (float)height}, 4, ORANGE);
	}

	if (cards.empty()) {
		return;
	}

	int fontSize = 20;
	int spacing = 20;

	int offsetY = height / 2 - fontSize * 2 - spacing / 2;

	Card *topCard = &cards.back();

	const char *name = topCard->name.c_str();

	int textWidth = MeasureText(name, fontSize);

	int textX = position.x + width / 2 - textWidth / 2;

	DrawText(topCard->name.c_str(), textX, position.y + offsetY, fontSize,
			 BLACK);

	const char *value = TextFormat("Val: %d", topCard->value);
	textWidth = MeasureText(value, fontSize);

	textX = position.x + width / 2 - textWidth / 2;

	DrawText(value, textX, position.y + offsetY + spacing, fontSize, DARKBLUE);
}
