#include "pile.hpp"
#include "textureManager.hpp"
#include "types.hpp"
#include <execution>
#include <raylib.h>

void Pile::Draw(TextureManager &tm, bool isSelected, bool isHighlited) {
	Vector2 renderPos = position;

	bool isMouseOver = CheckCollisionPointRec(
		GetMousePosition(), {position.x, position.y, static_cast<float>(width),
							 static_cast<float>(height)});

	float shadowOffset = 4.0f;

	if ((isMouseOver || isSelected) && !IsEmpty()) {
		renderPos.y -= 12.0f;
		shadowOffset = 14.0f;
	}

	float cLen = 14.0f;
	float cThick = 4.0f;
	float g = 4.0f;

	Color goldColor = {0, 0, 0, 0};
	bool drawCorners = false;

	if (isSelected) {
		goldColor = Color{230, 140, 30, 255};
		drawCorners = true;
	} else if (isHighlited) {
		goldColor = Color{40, 200, 110, 255};
		drawCorners = true;
	}

	if (drawCorners) {
		DrawLineEx({renderPos.x - g, renderPos.y - g},
				   {renderPos.x - g + cLen, renderPos.y - g}, cThick,
				   goldColor);
		DrawLineEx({renderPos.x - g, renderPos.y - g},
				   {renderPos.x - g, renderPos.y - g + cLen}, cThick,
				   goldColor);
		DrawLineEx({renderPos.x + width + g, renderPos.y - g},
				   {renderPos.x + width + g - cLen, renderPos.y - g}, cThick,
				   goldColor);
		DrawLineEx({renderPos.x + width + g, renderPos.y - g},
				   {renderPos.x + width + g, renderPos.y - g + cLen}, cThick,
				   goldColor);
		DrawLineEx({renderPos.x - g, renderPos.y + height + g},
				   {renderPos.x - g + cLen, renderPos.y + height + g}, cThick,
				   goldColor);
		DrawLineEx({renderPos.x - g, renderPos.y + height + g},
				   {renderPos.x - g, renderPos.y + height + g - cLen}, cThick,
				   goldColor);
		DrawLineEx({renderPos.x + width + g, renderPos.y + height + g},
				   {renderPos.x + width + g - cLen, renderPos.y + height + g},
				   cThick, goldColor);
		DrawLineEx({renderPos.x + width + g, renderPos.y + height + g},
				   {renderPos.x + width + g, renderPos.y + height + g - cLen},
				   cThick, goldColor);
	}

	if (cards.empty()) {

		DrawCardBackground(position);

		if (isLeftHand || isRightHand) {
			const Texture2D &handLTex = tm.get(TextureId::HandL);
			const Texture2D &handRTex = tm.get(TextureId::HandR);

			float emptyScale = 3.0f;

			float centerX = position.x + width / 2;
			float centerY = position.y + height / 2;

			float handX = centerX - (handLTex.width * emptyScale) / 2.0f;
			float handY = centerY - (handLTex.height * emptyScale) / 2.0f;

			if (isLeftHand)
				DrawTextureEx(handLTex, {handX, handY}, 0.0f, emptyScale,
							  Fade(RAYWHITE, 0.4f));
			if (isRightHand)
				DrawTextureEx(handRTex, {handX, handY}, 0.0f, emptyScale,
							  Fade(RAYWHITE, 0.4f));
		}
		if (isDiscardPile) {
			auto xColor = Fade(DARKGRAY, 0.6f);
			float thicknes = 5.0f;

			float centerX = position.x + width / 2.0f;
			float centerY = position.y + height / 2.0f;
			float size = 20.0f;

			DrawLineEx({centerX - size, centerY - size},
					   {centerX + size, centerY + size}, thicknes, xColor);
			DrawLineEx({centerX + size, centerY - size},
					   {centerX - size, centerY + size}, thicknes, xColor);
		}
		return;
	}

	float centerX = renderPos.x + width / 2.0f;
	float centerY = renderPos.y + height / 2.0f;

	DrawCardBackground(renderPos);

	const Texture2D &texture = tm.get(cards.back().textureId);
	float scale = 3.0F;
	if (cards.back().type == CardType::ENEMY ||
		cards.back().type == CardType::PLAYER) {
		scale = 5.0F;
	}
	if (cards.back().textureId == TextureId::ItemShield) {
		scale = 0.5F;
	}

	float drawX = centerX - (texture.width * scale) / 2.0f;
	float drawY = centerY - (texture.height * scale) / 2.0f;

	DrawTextureEx(texture, {drawX, drawY - 5 * scale}, 0.0f, scale, RAYWHITE);

	if ((cards.back().type == CardType::WEAPON ||
		 cards.back().type == CardType::WAND) &&
		cards.back().maxDurability > 0 && cards.back().maxDurability != -1) {
		int maxDur = cards.back().maxDurability;
		int curDur = cards.back().durability;

		float pipRadius = 5.0f;
		float pipSpacing = 14.0f;

		float totalWidth = (maxDur - 1) * pipSpacing;
		float startX = centerX - (totalWidth / 2.0f);

		float pipY = drawY + (texture.height * scale) + 8.0f;

		for (int i = 0; i < maxDur; i++) {
			Vector2 pipCenter = {startX + (i * pipSpacing), pipY};

			Color pipColor = (i < curDur) ? Color{230, 140, 30, 255}
										  : Color{60, 60, 50, 255};

			DrawPoly(pipCenter, 4, pipRadius, 0.0f, pipColor);

			DrawPolyLinesEx(pipCenter, 4, pipRadius, 0.0f, 1.0f,
							Color{20, 20, 20, 200});
		}
	}
}

void Pile::DrawCardBackground(Vector2 position) {
	float centerX = position.x + width / 2.0f;
	float centerY = position.y + height / 2.0f;

	const auto shadow = Color{0, 0, 0, 100};
	const auto darkWood = Color{60, 30, 15, 255};
	const auto parchment = Color{220, 200, 170, 255};
	const auto goldAccent = Color{180, 130, 40, 255};

	DrawRectangle(position.x + 4, position.y + 4, width, height, shadow);
	DrawRectangle(position.x, position.y, width, height, darkWood);

	const float border = 6;
	DrawRectangle(position.x + border, position.y + border,
				  width - (2 * border), height - (2 * border), parchment);

	int innerMargin = border + 4;
	DrawRectangleLinesEx({position.x + static_cast<float>(innerMargin),
						  position.y + static_cast<float>(innerMargin),
						  static_cast<float>(width - (2 * innerMargin)),
						  static_cast<float>(height - (innerMargin * 2))},
						 2, goldAccent);

	float cornerLen = 16.0F;
	float thick = 4.0F;
	float p = innerMargin;

	Vector2 tl = {position.x + p, position.y + p};
	DrawLineEx(tl, {tl.x + cornerLen, tl.y}, thick, goldAccent);
	DrawLineEx(tl, {tl.x, tl.y + cornerLen}, thick, goldAccent);

	Vector2 tr = {position.x + width - p, position.y + p};
	DrawLineEx(tr, {tr.x - cornerLen, tr.y}, thick, goldAccent);
	DrawLineEx(tr, {tr.x, tr.y + cornerLen}, thick, goldAccent);

	Vector2 bl = {position.x + p, position.y + height - p};
	DrawLineEx(bl, {bl.x + cornerLen, bl.y}, thick, goldAccent);
	DrawLineEx(bl, {bl.x, bl.y - cornerLen}, thick, goldAccent);

	Vector2 br = {position.x + width - p, position.y + height - p};
	DrawLineEx(br, {br.x - cornerLen, br.y}, thick, goldAccent);
	DrawLineEx(br, {br.x, br.y - cornerLen}, thick, goldAccent);

	const float niteRadius = 3;
	DrawCircle(tl.x, tl.y, niteRadius, darkWood);
	DrawCircle(tr.x, tr.y, niteRadius, darkWood);
	DrawCircle(bl.x, bl.y, niteRadius, darkWood);
	DrawCircle(br.x, br.y, niteRadius, darkWood);

	if (IsEmpty()) {
		return;
	}

	Card *topCard = &cards.back();

	int fontSize = 20;
	const char *name = topCard->name.c_str();
	int textWidth = MeasureText(name, fontSize);
	int texX = position.x + width / 2 - textWidth / 2;
	DrawText(name, texX, position.y + 16, fontSize, {40, 20, 10, 255});

	const float sepY = position.y + 42;
	DrawLineEx({position.x + 14, sepY}, {position.x + width - 14, sepY}, 2,
			   goldAccent);
	DrawCircle(position.x + 14, sepY, 2, darkWood);
	DrawCircle(position.x + width - 14, sepY, 2, darkWood);

	const char *valueStr;
	if (topCard->maxValue == -1 || topCard->minValue == -1) {
		valueStr = TextFormat("%d", topCard->hp);
	} else {
		valueStr = TextFormat("%d | %d", topCard->minValue, topCard->maxValue);
	}

	if (topCard->type == CardType::PLAYER) {
		valueStr = TextFormat("%d | %d", topCard->hp, topCard->maxValue);
	}

	const float sepYBottom = position.y + height - 46;
	DrawLineEx({position.x + 14, sepYBottom},
			   {position.x + width - 14, sepYBottom}, 2, goldAccent);
	DrawCircle(position.x + 14, sepYBottom, 2, darkWood);
	DrawCircle(position.x + width - 14, sepYBottom, 2, darkWood);

	fontSize = 24;
	textWidth = MeasureText(valueStr, fontSize);
	int valTextX = centerX - textWidth / 2;
	int valTextY = sepYBottom + 8;

	auto valueColor = Color{50, 30, 20, 255};
	if (topCard->type == CardType::ENEMY)
		valueColor = Color{150, 25, 25, 255};
	if (topCard->type == CardType::POTION)
		valueColor = Color{25, 110, 25, 255};

	DrawText(valueStr, valTextX, valTextY, fontSize, valueColor);
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
