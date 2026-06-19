#include "menu.hpp"
#include <raylib.h>

void Menu::Init() {
	gameTitle = "DungeonCards";

	UpdateButtonsPos();

	playButton.text = "PLAY";
}

void Menu::UpdateButtonsPos() {
	float playButtonW = 200;
	float playButtonH = 80;

	playButton.rect = {.x = GetScreenWidth() / 2.0f -
							playButton.rect.width / 2.0f,
					   .y = GetScreenHeight() / 2.0f,
					   .width = playButtonW,
					   .height = playButtonH};
}

MenuUpdateResoult Menu::Update() {
	UpdateButtonsPos();

	if (playButton.IsClicked(GetMousePosition())) {
		return MenuUpdateResoult::PLAY;
	}

	return MenuUpdateResoult::NONE;
}

void Menu::Draw() const {
	playButton.Draw();
}
