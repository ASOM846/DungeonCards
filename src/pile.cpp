#include "pile.hpp"
#include "types.hpp"
#include <raylib.h>

void Pile::Draw(bool &isSelected) {
	if (cards.size() > 1) {
		const char *countStr =
			TextFormat("Cards: %d", static_cast<int>(cards.size()));
		int countFontSize = 20;
		int countTextWidth = MeasureText(countStr, countFontSize);
		int countX = position.x + width / 2 - countTextWidth / 2;
		int countY = position.y + height + 10;

		DrawText(countStr, countX, countY, countFontSize, DARKGRAY);
	}
	if (cards.empty()) {
		DrawRectangleV(position,
					   {static_cast<float>(width), static_cast<float>(height)},
					   Color{40, 40, 40, 100});
		return;
	}

	int extraCards = cards.size() - 1;
	if (extraCards > 2)
		extraCards = 2;

	for (int i = extraCards; i > 0; i--) {
		float shiftY = -i * 12;
		Vector2 stackPos = {position.x, position.y + shiftY};
		DrawRectangleV(stackPos,
					   {static_cast<float>(width), static_cast<float>(height)},
					   WHITE);
		DrawRectangleLinesEx(
			{stackPos.x, stackPos.y, (float)width, (float)height}, 2,
			LIGHTGRAY);
	}

	DrawRectangleV(position,
				   {static_cast<float>(width), static_cast<float>(height)},
				   WHITE);

	DrawRectangleLinesEx({position.x, position.y, (float)width, (float)height},
						 2, LIGHTGRAY);

	if (isSelected) {
		DrawRectangleLinesEx(
			{position.x, position.y, (float)width, (float)height}, 4, ORANGE);
	}

	Card *topCard = &cards.back();
	int fontSize = 20;

	float centerX = position.x + width / 2.0f;
	float centerY = position.y + height / 2.0f;

	switch (topCard->type) {
	case CardType::ENEMY: {
		if (topCard->element == Element::NONE) {
			DrawEnemy(centerX, centerY, GREEN);
		} else if (topCard->element == Element::FIRE) {
			DrawEnemy(centerX, centerY, RED);
		} else {
			DrawEnemy(centerX, centerY, BLUE);
		}
		break;
	}
	case CardType::WEAPON: {
		DrawRectangle(centerX - 6, centerY - 45, 12, 55, LIGHTGRAY);
		DrawRectangleLines(centerX - 6, centerY - 45, 12, 55, DARKGRAY);
		DrawTriangle({centerX - 6, centerY - 45}, {centerX, centerY - 57},
					 {centerX + 6, centerY - 45}, LIGHTGRAY);
		DrawRectangle(centerX - 22, centerY + 10, 44, 8, GOLD);
		DrawRectangle(centerX - 4, centerY + 18, 8, 20, BROWN);
		DrawCircle(centerX, centerY + 40, 6, GOLD);
		break;
	}
	case CardType::WAND: {
		DrawRectangle(centerX - 4, centerY - 10, 8, 45, LIGHTGRAY);
		DrawRectangle(centerX - 8, centerY - 15, 16, 6, SKYBLUE);
		DrawTriangle({centerX - 14, centerY - 30}, {centerX, centerY - 52},
					 {centerX + 14, centerY - 30}, SKYBLUE);
		DrawTriangle({centerX - 14, centerY - 30}, {centerX + 14, centerY - 30},
					 {centerX, centerY - 8}, SKYBLUE);
		DrawTriangle({centerX - 7, centerY - 30}, {centerX, centerY - 42},
					 {centerX + 7, centerY - 30}, WHITE);
		DrawTriangle({centerX - 7, centerY - 30}, {centerX + 7, centerY - 30},
					 {centerX, centerY - 18}, WHITE);
		break;
	}
	// case CardType::WAND_FIRE: {
	// 	DrawRectangle(centerX - 4, centerY - 10, 8, 45, BROWN);
	// 	DrawRectangle(centerX - 8, centerY - 15, 16, 6, GOLD);
	// 	DrawCircle(centerX, centerY - 30, 16, RED);
	// 	DrawCircle(centerX, centerY - 26, 10, ORANGE);
	// 	DrawTriangle({centerX - 10, centerY - 25}, {centerX, centerY - 48},
	// 				 {centerX + 10, centerY - 25}, ORANGE);
	// 	DrawCircle(centerX, centerY - 24, 5, YELLOW);
	// 	break;
	// }
	case CardType::POTION: {
		DrawCircle(centerX, centerY + 15, 30, DARKGRAY);
		DrawCircle(centerX, centerY + 15, 27, LIME);
		DrawRectangle(centerX - 8, centerY - 25, 16, 20, DARKGRAY);
		DrawRectangle(centerX - 6, centerY - 23, 12, 22, LIGHTGRAY);
		DrawRectangle(centerX - 10, centerY - 32, 20, 8, BROWN);
		DrawCircle(centerX - 10, centerY + 5, 4, WHITE);
		break;
	}
	case CardType::PLAYER: {
		Vector2 p1 = {centerX, centerY - 40};
		Vector2 p2 = {centerX - 30, centerY - 10};
		Vector2 p3 = {centerX + 30, centerY - 10};
		DrawTriangle(p1, p2, p3, BLUE);
		DrawRectangle(centerX - 30, centerY - 10, 60, 30, BLUE);
		DrawTriangle({centerX - 30, centerY + 20}, {centerX, centerY + 45},
					 {centerX + 30, centerY + 20}, BLUE);
		DrawLineEx({centerX, centerY - 25}, {centerX, centerY + 25}, 4, GOLD);
		DrawLineEx({centerX - 15, centerY}, {centerX + 15, centerY}, 4, GOLD);
		break;
	}
	default:
		break;
	}

	const char *name = topCard->name.c_str();
	int textWidth = MeasureText(name, fontSize);
	int textX = position.x + width / 2 - textWidth / 2;
	DrawText(name, textX, position.y + 15, fontSize, BLACK);

	const char *valueStr = TextFormat("%d", topCard->value);
	fontSize = 24;
	textWidth = MeasureText(valueStr, fontSize);
	textX = position.x + width / 2 - textWidth / 2;

	auto valueColor = DARKBLUE;
	if (topCard->type == CardType::ENEMY)
		valueColor = RED;
	if (topCard->type == CardType::POTION)
		valueColor = GREEN;

	DrawText(valueStr, textX, position.y + height - 35, fontSize, valueColor);
}

void Pile::DrawEnemy(const int &centerX, const int &centerY, Color color) {
	DrawCircle(centerX, centerY, 35, color);
	DrawCircle(centerX - 12, centerY - 10, 6, WHITE);
	DrawCircle(centerX + 12, centerY - 10, 6, WHITE);
	DrawCircle(centerX - 12, centerY - 10, 2, BLACK);
	DrawCircle(centerX + 12, centerY - 10, 2, BLACK);
	DrawLineEx(
		{static_cast<float>(centerX - 20), static_cast<float>(centerY - 20)},
		{static_cast<float>(centerX - 5), static_cast<float>(centerY - 13)}, 3,
		BLACK);
	DrawLineEx(
		{static_cast<float>(centerX + 20), static_cast<float>(centerY - 20)},
		{static_cast<float>(centerX + 5), static_cast<float>(centerY - 13)}, 3,
		BLACK);
	DrawRectangle(centerX - 15, centerY + 10, 30, 8, BLACK);
}
