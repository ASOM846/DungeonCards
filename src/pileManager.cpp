#include "pileManager.hpp"
#include "interactionManager.hpp"
#include "pile.hpp"
#include "textureManager.hpp"
#include "types.hpp"
#include <algorithm>
#include <random>
#include <ratio>
#include <raylib.h>
#include <sys/types.h>
#include <vector>

void PileManager::Reset() {
	for (auto &dungeonPile : dungeonPiles) {
		dungeonPile.cards.clear();
	}

	for (auto &playerPile : playerPiles) {
		playerPile.cards.clear();
	}

	discardPile[0].cards.clear();

	masterDeck.clear();
}

void PileManager::Init() {

	Reset();

	UpdatePilesOffset();

	struct CardTemplate {
		CardType type;
		Element element;
	};

	std::vector<CardTemplate> finalCardPool;

	int totalCardsNeeded = 64;

	int packSize = 16;
	int numPacks = totalCardsNeeded / packSize;

	for (int pack = 0; pack < numPacks; pack++) {
		for (int i = 0; i < 4; i++)
			finalCardPool.push_back({CardType::ENEMY, Element::NONE});
		for (int i = 0; i < 2; i++)
			finalCardPool.push_back({CardType::ENEMY, Element::ICE});
		for (int i = 0; i < 2; i++)
			finalCardPool.push_back({CardType::ENEMY, Element::FIRE});

		for (int i = 0; i < 1; i++)
			finalCardPool.push_back({CardType::POTION, Element::NONE});

		for (int i = 0; i < 2; i++)
			finalCardPool.push_back({CardType::WEAPON, Element::NONE});

		for (int i = 0; i < 2; i++) {
			finalCardPool.push_back({CardType::SPELL, Element::NONE});
		}

		for (int i = 0; i < 1; i++)
			finalCardPool.push_back({CardType::SHIELD, Element::NONE});

		Element wandElement =
			(GetRandomValue(0, 1) == 0) ? Element::FIRE : Element::ICE;
		finalCardPool.push_back({CardType::WAND, wandElement});

		finalCardPool.push_back({CardType::COIN, Element::NONE});
	}

	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(finalCardPool.begin(), finalCardPool.end(), g);

	for (auto &tmp : finalCardPool) {
		Card card;

		if (tmp.type == CardType::SPELL) {
			card = GetRandomSpell();
			TraceLog(LOG_INFO, "SPECIAL_WEAPON INITIALIZED");

		} else {
			card = GenerateCardData(tmp.type, tmp.element);

			if (card.type == CardType::ENEMY)
				card.value = GetRandomValue(2, 10);
			else if (card.type == CardType::POTION)
				card.value = GetRandomValue(2, 10);
			else if (card.type == CardType::COIN)
				card.value = GetRandomValue(1, 12);
			else
				card.value = GetRandomValue(3, 9);
		}
		masterDeck.push_back(card);
	}

	Card sword = {.name = "SWORD",
				  .value = 5,
				  .type = CardType::WEAPON,
				  .textureId = TextureId::WeaponSword};

	dungeonPiles[D_ONE].cards.push_back(sword);

	Card player = {.name = "PLAYER",
				   .value = 13,
				   .maxValue = 13,
				   .type = CardType::PLAYER,
				   .textureId = TextureId::Knight1};

	playerPiles[P_PLAYER].cards.push_back(player);
}

void PileManager::DrawAll(TextureManager &tm, Pile *selectedPile) {
	for (auto &pile : dungeonPiles) {
		bool isSel = (&pile == selectedPile);
		bool isHigh = InteractionManager::ShouldHighlight(selectedPile, &pile);

		pile.Draw(tm, isSel, isHigh);
	}

	for (auto &pile : playerPiles) {
		bool isSel = (&pile == selectedPile);
		bool isHigh = InteractionManager::ShouldHighlight(selectedPile, &pile);

		pile.Draw(tm, isSel, isHigh);
	}

	bool isDiscard = false;
	discardPile[0].Draw(tm, isDiscard);
}

void PileManager::UpdatePilesOffset() {
	int spacing = 20;
	int offsetY = 100;

	int screenWidth = GetScreenWidth();
	int screenHeight = GetScreenHeight();

	float totalHeight = (2.0f * pileHeight) + spacing;
	float startY = ((screenHeight - totalHeight) / 2.0f) - 30.0f;

	startY = std::max(startY, 40.0F);

	int totalWidth = (D_COUNT * pileWidth) + ((D_COUNT - 1) * spacing);
	int startX = (screenWidth - totalWidth) / 2;

	for (int i = 0; i < D_COUNT; i++) {
		dungeonPiles[i].position = {
			.x = (float)(startX + i * (pileWidth + spacing)), .y = startY};
		dungeonPiles[i].width = pileWidth;
		dungeonPiles[i].height = pileHeight;
		dungeonPiles[i].isDiscardPile = false;
		dungeonPiles[i].isDungeonPile = true;
	}

	discardPile[0].position = {
		.x = static_cast<float>(startX + totalWidth + spacing * 3),
		.y = startY - 25.0F};
	discardPile[0].width = pileWidth;
	discardPile[0].height = pileHeight;
	discardPile->isDiscardPile = true;

	float playerRowY = startY + pileHeight + spacing;

	for (int i = 0; i < P_COUNT; i++) {
		playerPiles[i].position = {
			.x = (float)(startX + i * (pileWidth + spacing)), .y = playerRowY};
		playerPiles[i].width = pileWidth;
		playerPiles[i].height = pileHeight;
		playerPiles[i].isDiscardPile = false;

		playerPiles[i].isLeftHand = false;
		playerPiles[i].isRightHand = false;
		playerPiles[i].isBackpack = false;

		if (i == P_LEFT) {
			playerPiles[i].isLeftHand = true;
		} else if (i == P_RIGHT) {
			playerPiles[i].isRightHand = true;
		} else if (i == P_BACKPACK) {
			playerPiles[i].isBackpack = true;
		}
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

Card *PileManager::GetCardAt(Vector2 mousePos) {
	Pile *temp = GetPileAt(mousePos);

	if (temp == nullptr) {
		return nullptr;
	}

	if (temp->cards.empty()) {
		return nullptr;
	}

	return &temp->cards.back();
}

void PileManager::RefillRoomIfNeeded() {
	int activeCards = 0;
	for (const auto &dungeonPile : dungeonPiles) {
		if (!dungeonPile.IsEmpty()) {
			activeCards++;
		}
	}

	if (activeCards <= 1) {
		for (auto &dungeonPile : dungeonPiles) {
			if (dungeonPile.IsEmpty() && !masterDeck.empty()) {
				dungeonPile.cards.push_back(masterDeck.back());
				masterDeck.pop_back();
			}
		}
	}
}

TextureId PileManager::GetRandomMonsterTexture() {
	static const std::vector<TextureId> textures = {
		TextureId::Enemy1, TextureId::Enemy2, TextureId::Enemy3,
		TextureId::Enemy4, TextureId::Enemy5, TextureId::Enemy6,
		TextureId::Enemy7, TextureId::Enemy8,
	};

	int seed = GetRandomValue(0, textures.size() - 1);

	return textures[seed];
}

Card PileManager::GetRandomSpell() {
	std::vector<Element> elements;
	elements.push_back(Element::LIFESTEAL);
	elements.push_back(Element::LIFESTEAL);

	elements.push_back(Element::WARHAMMER);
	elements.push_back(Element::ESCAPE);

	Element selectedElement;

	int seed = GetRandomValue(0, elements.size() - 1);

	selectedElement = elements[seed];

	Card c = GenerateCardData(CardType::SPELL, selectedElement);

	switch (selectedElement) {
	case Element::LIFESTEAL:
		TraceLog(LOG_INFO, "LIFESTEAL init");
		c.value = GetRandomValue(6, 12);
		break;
	case Element::WARHAMMER:
		TraceLog(LOG_INFO, "WARHAMMER init");
		c.value = 1;
		break;
	case Element::ESCAPE:
		TraceLog(LOG_INFO, "ESCAPE init");
		c.value = 1;
		break;
	}

	return c;
}

Card PileManager::GenerateCardData(const CardType type, const Element element) {
	Card c;
	c.type = type;
	c.element = element;

	switch (type) {
	case CardType::ENEMY:
		c.textureId = GetRandomMonsterTexture();

		if (c.element == Element::NONE) {
			c.name = "MONSTER";
			c.description = "MONSTER - STANDARD ENEMY";
		}
		if (c.element == Element::ICE) {
			c.name = "FROST";
			c.description = "FROST - DOUBLE DAMAGE FROM FIRE";
			c.textureId = TextureId::EnemyIce;
		}
		if (c.element == Element::FIRE) {
			c.name = "BLAZE";
			c.description = "BLAZE - DOUBLE DAMAGE FROM ICE";
			c.textureId = TextureId::EnemyFire;
		}
		break;
	case CardType::WEAPON:
		c.name = "SWORD";
		c.description = "SWORD - STANDARD ENEMY";
		c.textureId = TextureId::WeaponSword;
		break;
	case CardType::SPELL:
		c.name = "SPELL";
		c.description =
			"SPELL - YOU SHOUDL NOT BE ABLE TO HAVE THIS IN THIS FORM xd";
		if (c.element == Element::LIFESTEAL) {
			c.name = "LIFESTEAL";
			c.description = "LIFESTEAL - HEALS YOU WITH DAMAGE IT DEALS";
			c.textureId = TextureId::WeaponGoldenSword;
		}
		if (c.element == Element::WARHAMMER) {
			c.name = "WARHAMMER";
			c.description = "WARHAMMER - SEND CARD BACK TO THE MASTER DECK";
			c.textureId = TextureId::WeaponHammer;
		}
		if (c.element == Element::ESCAPE) {
			c.name = "ESCAPE";
			c.description = "ESCAPE - REDRAW 4 CARDS";
			c.textureId = TextureId::FlaskBlue;
		}
		break;
	case CardType::WAND:
		if (c.element == Element::ICE) {
			c.name = "ICE WAND";
			c.description = "ICE WAND - DEALS DOUBLE DAMAGE TO FIRE. DEALS NO "
							"DAMAGE TO ICE";
		}
		if (c.element == Element::FIRE) {
			c.name = "FIRE WAND";
			c.description = "FIRE WAND - DEALS DOUBLE DAMAGE TO ICE. DEALS NO "
							"DAMAGE TO FIRE";
			c.textureId = TextureId::WeaponWandFire;
		}
		break;
	case CardType::SHIELD:
		c.name = "SHIELD";
		c.description = "SHIELD - DAMAGE ABSORBTION ITEM";
		c.textureId = TextureId::ItemShield;
		break;
	case CardType::POTION:
		c.name = "HP Elixir";
		c.description = "POTION - HEALS YOU";
		c.textureId = TextureId::FlaskRed;
		break;
	case CardType::COIN:
		c.name = "COIN";
		c.textureId = TextureId::Coin;
		break;
	case CardType::PLAYER:
	case CardType::COUNT:
		break;
	}
	return c;
}
