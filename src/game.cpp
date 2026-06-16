#include "game.hpp"
#include "types.hpp"
#include <raylib.h>

void Game::Init() {
	Reset();
	pileManager.Init();
}

void Game::Reset() {
	score = 0;
	gameOver = false;
}

void Game::Run() {}

void Game::Update() {
	Pile &playerPile = pileManager.GetPlayerPile(P_PLAYER);
	if (!playerPile.cards.empty()) {
		if (playerPile.cards.back().value <= 0) {
			gameOver = true;
		}
	}

	if (!gameOver) {
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			Vector2 mousePos = GetMousePosition();
			Pile *clickedPile = pileManager.GetPileAt(mousePos);

			if (clickedPile != nullptr)
				HandleInteraction(clickedPile);
		}
	}

	if (gameOver) {
		if (IsKeyDown(KEY_SPACE)) {
			CloseWindow();
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

	ui.Draw(pileManager.GetPlayerPile(P_PLAYER).cards.back().value, score);
}

void Game::HandleInteraction(Pile *target) {
	if (selected == nullptr) {
		if (!target->cards.empty()) {
			selected = target;
		}
		return;
	}

	if (selected == target) {
		selected = nullptr;
		return;
	}

	if (target->cards.empty()) {
		Card &sel = selected->cards.back();

		if (sel.type == CardType::PLAYER) {
			selected = nullptr;
			return;
		}

		target->cards.push_back(selected->cards.back());
		selected->cards.pop_back();
		selected = nullptr;
		return;
	}

	Card &sel = selected->cards.back();
	Card &tar = target->cards.back();

	if (sel.type == CardType::WEAPON && tar.type == CardType::ENEMY) {
		if (sel.value >= tar.value) {
			score += tar.value;

			sel.value -= tar.value;

			target->cards.pop_back();

			if (sel.value <= 0) {
				selected->cards.pop_back();
			}
		} else {
			tar.value -= sel.value;
			selected->cards.pop_back();
		}
	} else if (sel.type == CardType::PLAYER && tar.type == CardType::ENEMY) {
		score += tar.value;

		sel.value -= tar.value;
		target->cards.pop_back();

	} else if (sel.type == CardType::POTION && tar.type == CardType::PLAYER) {
		tar.value += sel.value;
		selected->cards.pop_back();
	}

	selected = nullptr;
}
