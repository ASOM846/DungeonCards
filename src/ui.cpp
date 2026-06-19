#include "ui.hpp"
#include <execution>
#include <raylib.h>

void Ui::DrawBackground(const Texture2D &texture) {
	Rectangle src = {0, 0, static_cast<float>(texture.width),
					 static_cast<float>(texture.height)};

	Rectangle dst = {0, 0, static_cast<float>(GetScreenWidth()),
					 static_cast<float>(GetScreenHeight())};

	DrawTexturePro(texture, src, dst, {0, 0}, 0.0f, WHITE);
}

void Ui::DrawMessageBox(const Card *card, const int &score) {
	const char *text;
	if (card != nullptr) {
		text = card->description.c_str();
	} else {
		text = TextFormat("SCORE:  %d", score);
	}

	float fontSize = 20.0f;
	float fontSpacing = 1.0f;
	Font font = GetFontDefault();

	Vector2 textSize = MeasureTextEx(font, text, fontSize, fontSpacing);

	float paddingX = 24.0f;
	float paddingY = 20.0f;

	Rectangle dst;
	dst.width = textSize.x + (paddingX * 2);
	dst.height = textSize.y + (paddingY * 2);

	dst.x = 20;
	dst.y = GetScreenHeight() - dst.height - 20;

	DrawMessageRect(dst, text);
}

void Ui::DrawMessageRect(const Rectangle &dst, const char *text,
						 float shadowOffset, const Font &font) {

	const auto shadow = Color{0, 0, 0, 100};
	const auto darkWood = Color{60, 30, 15, 255};
	const auto parchment = Color{220, 200, 170, 255};
	const auto goldAccent = Color{180, 130, 40, 255};

	DrawRectangle(dst.x + shadowOffset, dst.y + shadowOffset, dst.width,
				  dst.height, shadow);
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

	if (font.texture.id != GetFontDefault().texture.id)
		fontSize = 40.0f;
	float fontSpacing = 1.0f;

	Vector2 textSize = MeasureTextEx(font, text, fontSize, fontSpacing);

	float textX = (dst.x + dst.width / 2.0f) - (textSize.x / 2.0f);
	float textY = (dst.y + dst.height / 2.0f) - (textSize.y / 2.0f);

	DrawTextEx(font, text, {textX, textY}, fontSize, fontSpacing, darkWood);
}

void Ui::DrawProgressBar(float maxVal, float currentVal) {
	const auto parchment = Color{220, 200, 170, 255};
	const auto darkWood = Color{60, 30, 15, 255};

	float barWidth = GetScreenWidth() * 0.66f;
	const float barHeight = 16.0f;

	float currentPercent = currentVal / maxVal;
	if (currentPercent >= 1.0f)
		currentPercent = 1.0f;
	if (currentPercent < 0.0f)
		currentPercent = 0.0f;

	Vector2 pos = {GetScreenWidth() / 2 - barWidth / 2, 50};

	DrawRectangle(pos.x, pos.y, barWidth, barHeight, parchment);

	float border = 10.0f;
	float innerWidth = barWidth - (border * 2);
	float innerHeight = barHeight - (border * 2);

	float currentWidth = barWidth * currentPercent;
	if (currentWidth > 0) {
		DrawRectangle(pos.x, pos.y, currentWidth, barHeight,
					  Color{180, 130, 40, 255});
	}

	DrawRectangleLinesEx(
		{pos.x - border / 2, pos.y, barWidth + border / 2, barHeight}, 4.0f,
		Color{60, 30, 15, 255});

	float pointerX = pos.x + border + currentWidth;
	float pointerY = pos.y + (barHeight / 2);

	float pSize = 24.0f;
	float pBorder = 3.0f;

	if (currentPercent > 0.0f) {
		DrawRectangle(pointerX - pSize / 2, pointerY - pSize / 2, pSize, pSize,
					  darkWood);

		float pInnerSize = pSize - pBorder * 2;
		DrawRectangleLines(pointerX - pInnerSize / 2, pointerY - pInnerSize / 2,
						   pInnerSize, pInnerSize, parchment);
	}
}

void Ui::DrawTitle(const char *text, const Font &font) {
	float fontSize = 60;
	float fontSpacing = 1;

	Vector2 textM = MeasureTextEx(font, text, fontSize, fontSpacing);

	Vector2 textPos = {GetScreenWidth() / 2 - textM.x / 2,
					   static_cast<float>(GetScreenHeight() - fontSize * 1.5)};

	DrawTextEx(font, text, {textPos.x + 4.0f, textPos.y + 4.0f}, fontSize,
			   fontSpacing, Color{0, 0, 0, 100});
	DrawTextEx(font, text, textPos, fontSize, fontSpacing,
			   CLITERAL(Color){245, 237, 215, 255});
}
