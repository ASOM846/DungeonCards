#include "pileManager.hpp"
#include "types.hpp"
#include <algorithm>
#include <random>
#include <ratio>
#include <raylib.h>
#include <vector>

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

	discardPile[0].cards.clear();

	int totalWidth = (D_COUNT * pileWidth) + ((D_COUNT - 1) * spacing);
	int startX = (screenWidth - totalWidth) / 2;

	for (int i = 0; i < D_COUNT; i++) {
		dungeonPiles[i].position = {
			.x = (float)(startX + i * (pileWidth + spacing)),
			.y = 20.0f + offsetY};
		dungeonPiles[i].width = pileWidth;
		dungeonPiles[i].height = pileHeight;
	}

	discardPile[0].position = {
		.x = static_cast<float>(startX + totalWidth + spacing * 3),
		.y = static_cast<float>(20.0f + offsetY) - 25.0f};
	discardPile[0].width = pileWidth;
	discardPile[0].height = pileHeight;
	discardPile->isDiscardPile = true;

	totalWidth = (P_COUNT * pileWidth) + ((P_COUNT - 1) * spacing);
	startX = (screenWidth - totalWidth) / 2;

	for (int i = 0; i < P_COUNT; i++) {
		playerPiles[i].position = {
			.x = (float)(startX + i * (pileWidth + spacing)),
			.y = 310.0f + offsetY};
		playerPiles[i].width = pileWidth;
		playerPiles[i].height = pileHeight;
	}

	struct CardTemplate {
		CardType type;
		Element element;
	};

	std::vector<CardTemplate> finalCardPool;

	int cardsPerPile = 32;
	int totalCardsNeeded = D_COUNT * cardsPerPile;

	int packSize = 16;
	int numPacks = totalCardsNeeded / packSize;

	std::random_device rd;
	std::mt19937 g(rd());

	for (int pack = 0; pack < numPacks; pack++) {
		std::vector<CardTemplate> microPack;

		for (int i = 0; i < 5; i++)
			microPack.push_back({CardType::ENEMY, Element::NONE});
		for (int i = 0; i < 2; i++)
			microPack.push_back({CardType::ENEMY, Element::ICE});
		for (int i = 0; i < 2; i++)
			microPack.push_back({CardType::ENEMY, Element::FIRE});

		for (int i = 0; i < 3; i++)
			microPack.push_back({CardType::POTION, Element::NONE});
		for (int i = 0; i < 2; i++)
			microPack.push_back({CardType::WEAPON, Element::NONE});
		for (int i = 0; i < 1; i++)
			microPack.push_back({CardType::SHIELD, Element::NONE});

		Element wandElement =
			(GetRandomValue(0, 1) == 0) ? Element::FIRE : Element::ICE;
		microPack.push_back({CardType::WAND, wandElement});

		std::shuffle(microPack.begin(), microPack.end(), g);

		finalCardPool.insert(finalCardPool.end(), microPack.begin(),
							 microPack.end());
	}

	int poolIndex = 0;
	for (auto &i : dungeonPiles) {
		for (int j = 0; j < cardsPerPile; j++) {
			if (poolIndex >= finalCardPool.size())
				break;

			CardTemplate currentTemplate = finalCardPool[poolIndex++];
			Card card =
				GenerateCard(currentTemplate.type, currentTemplate.element);

			int stage = ((31 - j) / 6) + 1;

			if (card.type == CardType::ENEMY) {
				card.value = GetRandomValue(stage, stage + 2);
			} else if (card.type == CardType::POTION) {
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
	Card shield = {.name = "SHIELD",
				   .value = 10,
				   .type = CardType::SHIELD,
				   .element = Element::NONE};

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

	bool isDiscard = false;
	discardPile[0].Draw(isDiscard);
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

	{
		Rectangle rect = {discardPile[0].position.x, discardPile[0].position.y,
						  static_cast<float>(pileWidth),
						  static_cast<float>(pileHeight)};

		if (CheckCollisionPointRec(mousePos, rect)) {
			return &discardPile[0];
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
		break;
	case CardType::WEAPON:
		c.name = "SWORD";
		break;
	case CardType::WAND:
		if (c.element == Element::ICE)
			c.name = "ICE WAND";
		if (c.element == Element::FIRE)
			c.name = "FIRE WAND";
		break;
	case CardType::SHIELD:
		c.name = "SHIELD";
		break;
	case CardType::POTION:
		c.name = "POTION - HP";
		break;
	case CardType::PLAYER:
	case CardType::COUNT:
		break;
	}
	return c;
}
