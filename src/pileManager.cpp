#include "pileManager.hpp"
#include "types.hpp"
#include <raylib.h>

void PileManager::Init() {
	int spacing = 20;
	int offsetY = 100;

	int screenWidth = GetScreenWidth();
	int screenHeight = GetScreenHeight();

	for (auto &dungeonPile : dungeonPiles) {
		dungeonPile.cards.clear();
	}

	for (auto &playerPile : playerPiles) {
		playerPile.cards.clear();
	}

	int totalWidth = (D_COUNT * pileWidth) + ((D_COUNT - 1) * spacing);
	int startX = (screenWidth - totalWidth) / 2;

	for (int i = 0; i < D_COUNT; i++) {
		dungeonPiles[i].position = {
			.x = (float)(startX + i * (pileWidth + spacing)),
			.y = 20.0f + offsetY};
		dungeonPiles[i].width = pileWidth;
		dungeonPiles[i].height = pileHeight;
	}

	totalWidth = (P_COUNT * pileWidth) + ((P_COUNT - 1) * spacing);
	startX = (screenWidth - totalWidth) / 2;

	for (int i = 0; i < P_COUNT; i++) {
		playerPiles[i].position = {
			.x = (float)(startX + i * (pileWidth + spacing)),
			.y = 310.0f + offsetY};
		playerPiles[i].width = pileWidth;
		playerPiles[i].height = pileHeight;
	}

	for (auto &i : dungeonPiles) {
		for (int j = 0; j < 32; j++) {
			int roll = GetRandomValue(0, 99);
			CardType selectedType;
			Element selectedElement = Element::NONE;

			if (roll < 35) {
				selectedType = CardType::ENEMY;
			} else if (roll < 47) {
				selectedType = CardType::ENEMY;
				selectedElement = Element::ICE;
			} else if (roll < 59) {
				selectedType = CardType::ENEMY;
				selectedElement = Element::FIRE;
			} else if (roll < 74) {
				selectedType = CardType::POTION;
			} else if (roll < 88) {
				selectedType = CardType::WEAPON;
			} else if (roll < 94) {
				selectedType = CardType::WAND;
				selectedElement = Element::FIRE;
			} else {
				selectedType = CardType::WAND;
				selectedElement = Element::ICE;
			}

			Card card = GenerateCard(selectedType, selectedElement);

			int depth = 31 - j;
			int stage = (depth / 6) + 1;

			if (selectedType == CardType::ENEMY) {
				card.value = GetRandomValue(stage, stage + 2);
			} else if (selectedType == CardType::POTION) {
				card.value = GetRandomValue(2, stage + 4);
			} else {
				card.value = GetRandomValue(stage + 1, stage + 3);
			}

			i.cards.push_back(card);
		}
	}

	Card player = {.name = "PLAYER", .value = 10, .type = CardType::PLAYER};
	Card hpPotion = {.name = "HP POTION", .value = 4, .type = CardType::POTION};
	Card sword = {.name = "SWORD", .value = 8, .type = CardType::WEAPON};
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

	return GenerateCard(randomType, Element::NONE);
}

Card PileManager::GenerateCard(const CardType type, const Element element) {
	Card c;
	c.type = type;
	c.element = element;

	switch (type) {
	case CardType::ENEMY:
		c.name = "MONSTER";

		if (c.element == Element::ICE)
			c.name = "ICE\nMONSTER";
		if (c.element == Element::FIRE)
			c.name = "FIRE\nMONSTER";

		c.value = GetRandomValue(1, 8);
		break;
	case CardType::WEAPON:
		c.name = "SWORD";
		c.value = GetRandomValue(2, 5);
		break;
	case CardType::WAND:
		if (c.element == Element::ICE)
			c.name = "ICE WAND";
		if (c.element == Element::FIRE)
			c.name = "FIRE WAND";

		c.value = GetRandomValue(4, 10);
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
