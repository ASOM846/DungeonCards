#include "game.hpp"
#include "interactionManager.hpp"
#include "types.hpp"
#include <raylib.h>

void Game::Init() {
	Reset();
}

void Game::Reset() {
	score = 0;
	gameOver = false;

	pileManager.Init();
}

void Game::Run() {}

void Game::Update() {
	Pile &playerPile = pileManager.GetPlayerPile(P_PLAYER);
	if (!playerPile.IsEmpty()) {
		if (playerPile.Back().value <= 0) {
			gameOver = true;
		}
	}

	if (!gameOver) {
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			Vector2 mousePos = GetMousePosition();
			Pile *clickedPile = pileManager.GetPileAt(mousePos);

			if (clickedPile != nullptr) {
				InteractionManager::Handle(selected, clickedPile, score);
			}
		}
	}

	if (gameOver) {
		if (IsKeyDown(KEY_SPACE)) {
			Reset();
		}
	}
}

void Game::Draw() {

	pileManager.DrawAll(selected);

	if (gameOver) {
		int barHeight = 100;
		int barY = GetScreenHeight() / 2 - barHeight / 2;

		DrawRectangle(0, barY, GetScreenWidth(), barHeight,
					  Color{0, 0, 0, 200});

		const char *measage = "GAME LOST";
		int fontSize = 30;
		int textWidth = MeasureText(measage, fontSize);

		int textY = barY + barHeight / 2 - fontSize / 2;
		int textX = GetScreenWidth() / 2 - textWidth / 2;

		DrawText(measage, textX, textY, fontSize, RED);

		const char *meseage2 = "PRESS SPACE TO EXIT";
		fontSize = 20;
		textWidth = MeasureText(meseage2, fontSize);

		textY = barY + barHeight / 2 - fontSize / 2;
		textX = GetScreenWidth() / 2 - textWidth / 2;

		DrawText(meseage2, textX, textY + 30, fontSize, RED);
	}

	if (!pileManager.GetPlayerPile(P_PLAYER).IsEmpty()) {
		Ui::Draw(pileManager.GetPlayerPile(P_PLAYER).cards.back().value, score);
	}
}
