#include "game.hpp"
#include "types.hpp"
#include <raylib.h>

void Game::Init() {
	Reset();
}

void Game::Reset() {
	score = 0;
	gameOver = false;

	int spacing = 20;

	int screenWidth = GetScreenWidth();
	int screenHeight = GetScreenHeight();

	int totalWidth = (D_COUNT * pileWidth) + ((D_COUNT - 1) * spacing);
	int startX = (screenWidth - totalWidth) / 2;

	for (int i = 0; i < D_COUNT; i++) {
		state.dungeonPiles[i].position = {
			.x = (float)(startX + i * (pileWidth + spacing)), .y = 20.0f};
		state.dungeonPiles[i].width = pileWidth;
		state.dungeonPiles[i].height = pileHeight;
	}

	totalWidth = (P_COUNT * pileWidth) + ((P_COUNT - 1) * spacing);
	startX = (screenWidth - totalWidth) / 2;

	for (int i = 0; i < P_COUNT; i++) {
		state.playerPiles[i].position = {
			.x = (float)(startX + i * (pileWidth + spacing)), .y = 300.0f};
		state.playerPiles[i].width = pileWidth;
		state.playerPiles[i].height = pileHeight;
	}

	for (auto &i : state.dungeonPiles) {
		for (int j = 0; j < 32; j++) {
			int seed = GetRandomValue(0, 4);
			if (seed == 0 || seed == 1 || seed == 2) {
				i.cards.push_back(GenerateCard(CardType::ENEMY));
			} else if (seed == 3) {
				i.cards.push_back(GenerateCard(CardType::POTION));
			} else {
				i.cards.push_back(GenerateCard(CardType::WEAPON));
				TraceLog(LOG_INFO, "weapon spawned");
			}
		}
	}

	Card player = {.name = "PLAYER", .value = 10, .type = CardType::PLAYER};
	Card sword = {.name = "SWORD", .value = 8, .type = CardType::WEAPON};
	Card hpPotion = {.name = "HP POTION", .value = 4, .type = CardType::POTION};

	state.playerPiles[P_PLAYER].cards.push_back(player);
	state.playerPiles[P_RIGHT].cards.push_back(sword);
	state.playerPiles[P_BACKPACK].cards.push_back(hpPotion);
}

void Game::Run() {}

void Game::Update() {
	if (state.playerPiles[P_PLAYER].cards.back().value <= 0) {
		gameOver = true;
	}

	if (!gameOver) {
		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			Vector2 mousePos = GetMousePosition();

			for (auto &dungeonPile : state.dungeonPiles) {
				if (CheckCollisionPointRec(mousePos, {dungeonPile.position.x,
													  dungeonPile.position.y,
													  (float)pileWidth,
													  (float)pileHeight})) {
					HandleInteraction(selectedPile, dungeonPile, score);
				}
			}

			for (auto &playerPile : state.playerPiles) {
				if (CheckCollisionPointRec(
						mousePos, {playerPile.position.x, playerPile.position.y,
								   (float)pileWidth, (float)pileHeight})) {
					HandleInteraction(selectedPile, playerPile, score);
				}
			}
		}
	}

	if (IsKeyPressed(KEY_K)) {
		state.playerPiles[P_PLAYER].cards.back().value = 0;
	}

	if (gameOver) {
		if (IsKeyDown(KEY_SPACE)) {
			CloseWindow();
		}
	}
}

void Game::Draw() {
	for (auto &pile : state.dungeonPiles) {
		bool isSel = (&pile == selectedPile);
		pile.Draw(isSel);
	}

	for (auto &pile : state.playerPiles) {
		bool isSel = (&pile == selectedPile);
		pile.Draw(isSel);
	}

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

	ui.Draw(state.playerPiles[P_PLAYER].cards.back().value, score);
}

void Game::HandleInteraction(Pile *&selected, Pile &target, int &score) {
	if (selected == nullptr) {
		if (!target.cards.empty())
			selected = &target;
		return;
	}

	if (selected == &target) {
		selected = nullptr;
		return;
	}

	if (target.cards.empty()) {
		Card &sel = selected->cards.back();

		if (sel.type == CardType::PLAYER) {
			selected = nullptr;
			return;
		}

		target.cards.push_back(selected->cards.back());
		selected->cards.pop_back();
		selected = nullptr;
		return;
	}

	Card &sel = selected->cards.back();
	Card &tar = target.cards.back();

	if (sel.type == CardType::WEAPON && tar.type == CardType::ENEMY) {
		if (sel.value >= tar.value) {
			score += tar.value;

			sel.value -= tar.value;

			target.cards.pop_back();

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
		target.cards.pop_back();

	} else if (sel.type == CardType::POTION && tar.type == CardType::PLAYER) {
		tar.value += sel.value;
		selected->cards.pop_back();
	}

	selected = nullptr;
}

Card Game::GenerateRandomCard() {
	auto randomType = static_cast<CardType>(
		GetRandomValue(0, static_cast<int>(CardType::PLAYER) - 1));

	return GenerateCard(randomType);
}

Card Game::GenerateCard(const CardType type) {
	Card c;
	c.type = type;

	switch (type) {
	case CardType::ENEMY:
		c.name = "MONSTER";
		c.value = GetRandomValue(1, 8);
		break;
	case CardType::WEAPON:
		c.name = "WEAPON";
		c.value = GetRandomValue(2, 5);
		break;
	case CardType::POTION:
		c.name = "POTION - HP";
		c.value = GetRandomValue(2, 6);
		break;
	case CardType::PLAYER:
	case CardType::COUNT:
		break;
	}
	return c;
}
