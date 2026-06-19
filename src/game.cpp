#include "game.hpp"
#include "interactionManager.hpp"
#include "textureManager.hpp"
#include "types.hpp"
#include <iostream>
#include <raylib.h>

void Game::Init() {
	Reset();
	textureManager.loadAll();
	goBackButton.rect = {.x = static_cast<float>(GetScreenWidth() / 2),
						 .y = static_cast<float>(GetScreenHeight() / 2),
						 .width = 20,
						 .height = 20};
	goBackButton.text = "test";
}

void Game::Reset() {
	score = 0;
	cardsDefeated = 0;
	gameState = GameState::PLAYING;
	shouldReturnToMenu = false;

	pileManager.Init();
}

void Game::Update() {
	if (gameState == GameState::PLAYING) {

		Pile &playerPile = pileManager.GetPlayerPile(P_PLAYER);
		if (!playerPile.IsEmpty()) {
			if (playerPile.Back().value <= 0) {
				gameState = GameState::LOSE;
			}
		}

		if (pileManager.GetDungeonPile(D_ONE).IsEmpty() &&
			pileManager.GetDungeonPile(D_TWO).IsEmpty() &&
			pileManager.GetDungeonPile(D_THREE).IsEmpty() &&
			pileManager.GetDungeonPile(D_FOUR).IsEmpty()) {
			gameState = GameState::WIN;
		}
		pileManager.RefillRoomIfNeeded();
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			Vector2 mousePos = GetMousePosition();
			Pile *clickedPile = pileManager.GetPileAt(mousePos);

			if (clickedPile != nullptr) {
				InteractionManager::Handle(selected, clickedPile, score,
										   &pileManager.GetPlayerPile(P_PLAYER),
										   pileManager.GetDungeonPiles(),
										   pileManager.GetMasterDeck(),
										   cardsDefeated);
			}
		}
	}

	if (gameState != GameState::PLAYING) {
		if (IsKeyDown(KEY_SPACE)) {
			shouldReturnToMenu = true;
		}
	}

	int btnW = 150;
	int btnH = 70;
	int btnOffset = 20;

	goBackButton.rect = {
		.x = static_cast<float>(GetScreenWidth() - btnOffset - btnW),
		.y = static_cast<float>(GetScreenHeight() - btnOffset - btnH),
		.width = static_cast<float>(btnW),
		.height = static_cast<float>(btnH)};
	goBackButton.text = "Flee";

	goBackButton.Update(GetMousePosition());

	if (goBackButton.IsClicked(GetMousePosition())) {
		gameState = GameState::LOSE;
	}
}

void Game::Draw() {
	pileManager.DrawAll(textureManager, selected);

	if (gameState == GameState::LOSE) {
		DrawLose();
	}

	if (gameState == GameState::WIN) {
		DrawWin();
	}

	Ui::DrawMessageBox(pileManager.GetCardAt(GetMousePosition()));

	goBackButton.Draw(textureManager.getCustonFont());

	Ui::DrawProgressBar(pileManager.GetMasterDeckSize(), cardsDefeated);

	std::cout << cardsDefeated << "  /   " << pileManager.GetMasterDeckSize()
			  << std::endl;
}

void Game::DrawLose() {
	int barHeight = 100;
	int barY = GetScreenHeight() / 2 - barHeight / 2;

	DrawRectangle(0, barY, GetScreenWidth(), barHeight, Color{0, 0, 0, 200});

	const char *measage = "GAME LOST";
	int fontSize = 30;
	int textWidth = MeasureText(measage, fontSize);

	int textY = barY + barHeight / 2 - fontSize / 2;
	int textX = GetScreenWidth() / 2 - textWidth / 2;

	DrawText(measage, textX, textY, fontSize, RED);

	const char *meseage2 =
		TextFormat("PRESS SPACE TO RETURN TO MENU      SCORE:  %d", score);

	fontSize = 20;
	textWidth = MeasureText(meseage2, fontSize);

	textY = barY + barHeight / 2 - fontSize / 2;
	textX = GetScreenWidth() / 2 - textWidth / 2;

	DrawText(meseage2, textX, textY + 30, fontSize, RED);
}

void Game::DrawWin() {
	int barHeight = 100;
	int barY = GetScreenHeight() / 2 - barHeight / 2;

	DrawRectangle(0, barY, GetScreenWidth(), barHeight, Color{0, 0, 0, 200});

	const char *measage = "GAME WON";
	int fontSize = 30;
	int textWidth = MeasureText(measage, fontSize);

	int textY = barY + barHeight / 2 - fontSize / 2;
	int textX = GetScreenWidth() / 2 - textWidth / 2;

	DrawText(measage, textX, textY, fontSize, BLUE);

	const char *meseage2 =
		TextFormat("PRESS SPACE TO RETURN TO MENU      SCORE:  %d", score);

	fontSize = 20;
	textWidth = MeasureText(meseage2, fontSize);

	textY = barY + barHeight / 2 - fontSize / 2;
	textX = GetScreenWidth() / 2 - textWidth / 2;

	DrawText(meseage2, textX, textY + 30, fontSize, BLUE);
}
