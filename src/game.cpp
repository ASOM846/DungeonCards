#include "game.hpp"
#include "interactionManager.hpp"
#include "types.hpp"
#include <raylib.h>

void Game::Init() {
	Reset();
}

void Game::Reset() {
	score = 0;
	gameState = GameState::PLAYING;

	pileManager.Init();
}

void Game::Run() {}

void Game::Update() {
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

	if (gameState == GameState::PLAYING) {
		pileManager.RefillRoomIfNeeded();
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			Vector2 mousePos = GetMousePosition();
			Pile *clickedPile = pileManager.GetPileAt(mousePos);

			if (clickedPile != nullptr) {
				InteractionManager::Handle(
					selected, clickedPile, score,
					&pileManager.GetPlayerPile(P_PLAYER));
			}
		}
	}

	if (gameState != GameState::PLAYING) {
		if (IsKeyDown(KEY_SPACE)) {
			Reset();
		}
	}
}

void Game::Draw() {
	pileManager.DrawAll(selected);

	if (gameState == GameState::LOSE) {
		DrawLose();
	}

	if (gameState == GameState::WIN) {
		DrawWin();
	}

	if (!pileManager.GetPlayerPile(P_PLAYER).IsEmpty()) {
		Ui::Draw(pileManager.GetPlayerPile(P_PLAYER).cards.back().value, score,
				 pileManager.GetMasterDeckSize());
	}
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
		TextFormat("PRESS SPACE TO RESTART      SCORE:  %d", score);

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
		TextFormat("PRESS SPACE TO RESTART      SCORE:  %d", score);

	fontSize = 20;
	textWidth = MeasureText(meseage2, fontSize);

	textY = barY + barHeight / 2 - fontSize / 2;
	textX = GetScreenWidth() / 2 - textWidth / 2;

	DrawText(meseage2, textX, textY + 30, fontSize, BLUE);
}
