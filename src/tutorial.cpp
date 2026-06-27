#include "tutorial.hpp"
#include "menu.hpp"
#include "textureManager.hpp"
#include <raylib.h>

void TutorialState::Init() {
	UpdateFramePos();

	buttonForward.rect = {10, 10, 100, 50};
	buttonBack.rect = {300, 10, 100, 50};
	buttonReturn.rect = {500, 10, 100, 50};

	buttonForward.text = "Forward";
	buttonBack.text = "Back";
	buttonReturn.text = "Return";
}

void TutorialState::Reset() {
	returnToMenu = false;
}

void TutorialState::UpdateFramePos() {
	const float targetAspect = 1180.0f / 783.0f;

	float maxWidth = GetScreenWidth() * 2.0f / 3.0f;
	float maxHeight = GetScreenHeight() * 2.0f / 3.0f;

	float w = maxWidth;
	float h = w / targetAspect;

	if (h > maxHeight) {
		h = maxHeight;
		w = h * targetAspect;
	}

	float x = (GetScreenWidth() - w) / 2.0f;
	float y = (GetScreenHeight() - h) / 2.0f;

	imageDsc = {.x = static_cast<float>(x),
				.y = static_cast<float>(y),
				.width = static_cast<float>(w),
				.height = static_cast<float>(h)};
}

void TutorialState::Update(Vector2 mousePos) {
	if (IsWindowResized()) {
		UpdateFramePos();
	}

	int btnW = 200;
	int btnH = 80;
	int btnMargin = 20;

	buttonForward.rect = {
		.x = static_cast<float>(GetScreenWidth() - btnW - btnMargin),
		.y = static_cast<float>(GetScreenHeight() - btnH - btnMargin),
		.width = static_cast<float>(btnW),
		.height = static_cast<float>(btnH)};

	buttonBack.rect = {
		.x = static_cast<float>(btnMargin),
		.y = static_cast<float>(GetScreenHeight() - btnH - btnMargin),
		.width = static_cast<float>(btnW),
		.height = static_cast<float>(btnH)};
	buttonReturn.rect = {
		.x = (GetScreenWidth() - btnW) / 2.0f,
		.y = static_cast<float>(GetScreenHeight() - btnH - btnMargin),
		.width = static_cast<float>(btnW),
		.height = static_cast<float>(btnH)};

	buttonForward.Update(mousePos);
	buttonBack.Update(mousePos);
	buttonReturn.Update(mousePos);

	if (buttonForward.IsClicked(mousePos)) {
		currentImage++;
		if (currentImage > imageCount)
			currentImage = 0;
	}

	if (buttonBack.IsClicked(mousePos)) {
		currentImage--;
		if (currentImage < 0)
			currentImage = imageCount;
	}

	if (buttonReturn.IsClicked(mousePos)) {
		returnToMenu = true;
	}
}

void TutorialState::Draw(TextureManager &textureManager) {
	TextureId currImg;
	switch (currentImage) {
	case 0:
		currImg = TextureId::Tutorial1;
		break;
	case 1:
		currImg = TextureId::Tutorial2;
		break;
	case 2:
		currImg = TextureId::Tutorial3;
		break;
	case 3:
		currImg = TextureId::Tutorial4;
		break;
	}

	Texture2D texture = textureManager.get(currImg);

	Rectangle src = {0.0f, 0.0f, static_cast<float>(texture.width),
					 static_cast<float>(texture.height)};

	int border = 20;
	Rectangle borderRec = {imageDsc.x - border, imageDsc.y - border,
						   imageDsc.width + border * 2,
						   imageDsc.height + border * 2};

	Ui::DrawMessageRect(borderRec, "");

	Vector2 origin = {0, 0};

	DrawTexturePro(texture, src, imageDsc, origin, 0.0f, WHITE);

	buttonForward.Draw(textureManager.getCustonFont());
	buttonBack.Draw(textureManager.getCustonFont());
	buttonReturn.Draw(textureManager.getCustonFont());

	float fontSize = 60;
	float fontSpacing = 1;
	Font font = textureManager.getCustonFont();

	Vector2 textM = MeasureTextEx(font, "DungeonCards - How to play", fontSize,
								  fontSpacing);

	Vector2 textPos = {GetScreenWidth() / 2 - textM.x / 2,
					   static_cast<float>(fontSize / 2)};

	DrawTextEx(font, "DungeonCards - How to play",
			   {textPos.x + 4.0f, textPos.y + 4.0f}, fontSize, fontSpacing,
			   Color{0, 0, 0, 100});
	DrawTextEx(font, "DungeonCards - How to play", textPos, fontSize,
			   fontSpacing, CLITERAL(Color){245, 237, 215, 255});
}
