#include "game.hpp"
#include "floatingText.hpp"
#include "interactionManager.hpp"
#include "textureManager.hpp"
#include "types.hpp"
#include <raylib.h>

void Game::Init() {
	Reset();
	textureManager.loadAll();
	goBackButton.rect = {.x = static_cast<float>(GetScreenWidth() / 2),
						 .y = static_cast<float>(GetScreenHeight() / 2),
						 .width = 20,
						 .height = 20};
	goBackButton.text = "Flee";

	returnToMenuButton.rect = {.x = 0, .y = 0, .width = 20, .height = 20};

	returnToMenuButton.text = "Return to menu";
}

void Game::Reset() {
	score = 0;
	cardsDefeated = 0;
	gameState = GameState::PLAYING;
	shouldReturnToMenu = false;

	pileManager.Init();

	effectManager.Reset();
	screenShake.reset();

	boardCamera.target = {.x = 0.0F, .y = 0.0F};
	boardCamera.offset = {.x = 0.0F, .y = 0.0F};
	boardCamera.rotation = 0.0F;
	boardCamera.zoom = 1.0F;
}

void Game::Update() {
	if (gameState == GameState::PLAYING) {
		UpdatePlay();
	}

	if (gameState != GameState::PLAYING) {
		UpdateWinLose();
	}
}

void Game::Draw() {
	Ui::DrawTitle("DungeonCards", textureManager.getCustonFont());

	if (gameState == GameState::PLAYING) {
		boardCamera.offset = screenShake.offset;

		BeginMode2D(boardCamera);

		DrawPlay();

		EndMode2D();
	}

	if (gameState == GameState::LOSE || gameState == GameState::WIN) {
		DrawWinLose();
	}

	Ui::DrawProgressBar(pileManager.GetMasterDeckSize(), cardsDefeated);
}

void Game::UpdatePlay() {
	screenShake.update(GetFrameTime());

	effectManager.Update(GetFrameTime());

	Pile &playerPile = pileManager.GetPlayerPile(P_PLAYER);
	if (!playerPile.IsEmpty()) {
		if (playerPile.Back().hp <= 0) {
			gameState = GameState::LOSE;
		}
	}

	if (pileManager.GetDungeonPile(D_ONE).IsEmpty() &&
		pileManager.GetDungeonPile(D_TWO).IsEmpty() &&
		pileManager.GetDungeonPile(D_THREE).IsEmpty() &&
		pileManager.GetDungeonPile(D_FOUR).IsEmpty()) {
		gameState = GameState::WIN;
	}

	if (IsWindowResized()) {
		pileManager.UpdatePilesOffset();
	}

	pileManager.RefillRoomIfNeeded();
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		Vector2 mousePos = GetMousePosition();
		Pile *clickedPile = pileManager.GetPileAt(mousePos);

		if (clickedPile != nullptr) {
			InteractionContext ctx{
				.score = score,
				.cardsDefeated = cardsDefeated,
				.mousePos = GetMousePosition(),
				.playerPile = &pileManager.GetPlayerPile(P_PLAYER),
				.dungeonPiles = pileManager.GetDungeonPiles(),
				.masterDeck = pileManager.GetMasterDeck(),
				.screenShake = screenShake,
				.effectManager = effectManager};

			InteractionManager::Handle(selected, clickedPile, ctx);
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
void Game::DrawPlay() {
	pileManager.DrawAll(textureManager, selected);
	effectManager.Draw();

	Ui::DrawMessageBox(pileManager.GetCardAt(GetMousePosition()), score);

	goBackButton.Draw(textureManager.getCustonFont());
}

void Game::DrawWinLose() {
	const char *text = nullptr;

	if (gameState == GameState::WIN)
		text = TextFormat("Victory \n Score %d", score);
	else
		text = TextFormat("Defeat \n Score %d", score);

	int width = 300;
	int height = 300;

	Rectangle rect = {static_cast<float>(GetScreenWidth() / 2 - width / 2),
					  static_cast<float>(GetScreenHeight() / 2 - height / 2),
					  static_cast<float>(width), static_cast<float>(height)};

	Ui::DrawMessageRect(rect, text, 4.0F, textureManager.getCustonFont());

	returnToMenuButton.Draw(textureManager.getCustonFont());
}

void Game::UpdateWinLose() {
	int btnW = 300;
	int btnH = 100;

	returnToMenuButton.rect = {
		.x = static_cast<float>(GetScreenWidth() / 2 - btnW / 2),
		.y = static_cast<float>(GetScreenHeight() * 0.85f - btnH),
		.width = static_cast<float>(btnW),
		.height = static_cast<float>(btnH)};

	returnToMenuButton.Update(GetMousePosition());
	if (returnToMenuButton.IsClicked(GetMousePosition())) {
		shouldReturnToMenu = true;
	}
}
