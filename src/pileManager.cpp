#include "pileManager.hpp"
#include "types.hpp"
#include <raylib.h>

void PileManager::Init() {
	int spacing = 20;

	int screenWidth = GetScreenWidth();
	int screenHeight = GetScreenHeight();

	int totalWidth = (D_COUNT * pileWidth) + ((D_COUNT - 1) * spacing);
	int startX = (screenWidth - totalWidth) / 2;

	for (int i = 0; i < D_COUNT; i++) {
		dungeonPiles[i].position = {
			.x = (float)(startX + i * (pileWidth + spacing)), .y = 20.0f};
		dungeonPiles[i].width = pileWidth;
		dungeonPiles[i].height = pileHeight;
	}

	totalWidth = (P_COUNT * pileWidth) + ((P_COUNT - 1) * spacing);
	startX = (screenWidth - totalWidth) / 2;

	for (int i = 0; i < P_COUNT; i++) {
		playerPiles[i].position = {
			.x = (float)(startX + i * (pileWidth + spacing)), .y = 300.0f};
		playerPiles[i].width = pileWidth;
		playerPiles[i].height = pileHeight;
	}

	for (auto &i : dungeonPiles) {
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

	playerPiles[P_PLAYER].cards.push_back(player);
	playerPiles[P_RIGHT].cards.push_back(sword);
	playerPiles[P_BACKPACK].cards.push_back(hpPotion);
}

void PileManager::DrawAll(const Pile *selectedPile) {
	for (auto &pile : dungeonPiles) {
		bool isSel = (&pile == selectedPile);
		pile.Draw(isSel);
	}

	for (auto &pile : playerPiles) {
		bool isSel = (&pile == selectedPile);
		pile.Draw(isSel);
	}
}

Pile *PileManager::GetPileAt(Vector2 mousePos) {
	for (auto &dungeonPile : dungeonPiles) {
		Rectangle rect = {dungeonPile.position.x, dungeonPile.position.y,
						  (float)pileWidth, (float)pileHeight};
		if (CheckCollisionPointRec(mousePos, rect)) {
			return &dungeonPile;
		}
	}

	for (auto &playerPile : playerPiles) {
		Rectangle rect = {playerPile.position.x, playerPile.position.y,
						  (float)pileWidth, (float)pileHeight};

		if (CheckCollisionPointRec(mousePos, rect)) {
			return &playerPile;
		}
	}

	return nullptr;
}

Card PileManager::GenerateRandomCard() {
	auto randomType = static_cast<CardType>(
		GetRandomValue(0, static_cast<int>(CardType::PLAYER) - 1));

	return GenerateCard(randomType);
}

Card PileManager::GenerateCard(const CardType type) {
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
