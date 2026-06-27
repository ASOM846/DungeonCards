#include "menu.hpp"
#include "textureManager.hpp"
#include <raylib.h>

void Menu::Init() {
	gameTitle = "DungeonCards";

	UpdateButtonsPos();

	playButton.text = "Play";
	tutorialButton.text = "How to play";
}

void Menu::UpdateButtonsPos() {
	float playButtonW = 200;
	float playButtonH = 80;

	playButton.rect = {.x = GetScreenWidth() / 2.0f -
							playButton.rect.width / 2.0f,
					   .y = GetScreenHeight() / 2.0f,
					   .width = playButtonW,
					   .height = playButtonH};

	float tutorialButtonW = 200;
	float tutorialButtonH = 80;
	float gap = 20;

	tutorialButton.rect = {
		.x = GetScreenWidth() / 2.0f - tutorialButtonW / 2.0f,
		.y = playButton.rect.y + playButton.rect.height + gap,
		.width = tutorialButtonW,
		.height = tutorialButtonH};
}

MenuUpdateResoult Menu::Update() {
	UpdateButtonsPos();

	playButton.Update(GetMousePosition());
	tutorialButton.Update(GetMousePosition());

	if (playButton.IsClicked(GetMousePosition())) {
		return MenuUpdateResoult::PLAY;
	}

	if (tutorialButton.IsClicked(GetMousePosition())) {
		return MenuUpdateResoult::TUTORIAL;
	}

	return MenuUpdateResoult::NONE;
}

void Menu::Draw(TextureManager &tm) const {
	DrawTitle(tm);

	playButton.Draw(tm.getCustonFont());
	tutorialButton.Draw(tm.getCustonFont());
}

void Menu::DrawTitle(TextureManager &tm) const {
	float fontSize = 120;
	float fontSpacing = 1;
	Font font = tm.getCustonFont();

	Vector2 textM = MeasureTextEx(font, gameTitle, fontSize, fontSpacing);

	Vector2 textPos = {GetScreenWidth() / 2 - textM.x / 2,
					   static_cast<float>(GetScreenHeight() / 3)};

	DrawTextEx(font, gameTitle, {textPos.x + 4.0f, textPos.y + 4.0f}, fontSize,
			   fontSpacing, Color{0, 0, 0, 100});
	DrawTextEx(font, gameTitle, textPos, fontSize, fontSpacing,
			   CLITERAL(Color){245, 237, 215, 255});
}
