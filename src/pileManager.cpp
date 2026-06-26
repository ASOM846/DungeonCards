#include "pileManager.hpp"
#include "cardGenerator.hpp"
#include "interactionManager.hpp"
#include "pile.hpp"
#include "textureManager.hpp"
#include "types.hpp"
#include <algorithm>
#include <random>
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

	cardsInDeck = 60;

	int packSize = 15;
	int numPacks = cardsInDeck / packSize;

	for (int pack = 0; pack < numPacks; pack++) {
		for (int i = 0; i < 4; i++)
			finalCardPool.push_back({CardType::ENEMY, Element::NONE});
		for (int i = 0; i < 2; i++)
			finalCardPool.push_back({CardType::ENEMY, Element::ICE});
		for (int i = 0; i < 2; i++)
			finalCardPool.push_back({CardType::ENEMY, Element::FIRE});

		if (GetRandomValue(0, 1) == 0) {
			for (int i = 0; i < 1; i++)
				finalCardPool.push_back({CardType::POTION, Element::NONE});
		} else {
			for (int i = 0; i < 1; i++) {
				Element upgradeElement =
					(GetRandomValue(0, 1) != 0) ? Element::FIRE : Element::ICE;
				finalCardPool.push_back(
					{CardType::WEAPON_UPGRADE, upgradeElement});
			}
		}

		for (int i = 0; i < 1; i++)
			finalCardPool.push_back({CardType::WEAPON, Element::NONE});

		for (int i = 0; i < 1; i++) {
			finalCardPool.push_back({CardType::SPELL, Element::NONE});
		}

		for (int i = 0; i < 1; i++)
			finalCardPool.push_back({CardType::SHIELD, Element::NONE});

		Element wandElement =
			(GetRandomValue(0, 1) == 0) ? Element::FIRE : Element::ICE;
		finalCardPool.push_back({CardType::WAND, wandElement});

		for (int i = 0; i < 2; i++) {
			finalCardPool.push_back({CardType::COIN, Element::NONE});
		}
	}
	finalCardPool.push_back({CardType::CHEST, Element::NONE});
	finalCardPool.push_back({CardType::KEY, Element::NONE});
	finalCardPool.push_back({CardType::COIN, Element::NONE});
	finalCardPool.push_back({CardType::WEAPON_UPGRADE, Element::ANVIL});

	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(finalCardPool.begin(), finalCardPool.end(), g);

	for (auto &tmp : finalCardPool) {
		Card card;

		card = CardGenerator::GetItemParms(tmp.type, tmp.element);
		card.type = tmp.type;
		card.element = tmp.element;

		card.maxDurability = -1;
		card.durability = -1;

		switch (tmp.type) {
		case (CardType::SPELL): {
			card = CardGenerator::GetRandomSpell();
			break;
		}
		case (CardType::ENEMY): {
			card = CardGenerator::GetRandomEnemy(tmp.element);
			break;
		}
		case (CardType::POTION): {
			card.name = "ELIXIR";
			card.minValue = GetRandomValue(2, 4);
			card.maxValue = GetRandomValue(4, 8);
			break;
		}
		case (CardType::COIN): {
			card.minValue = GetRandomValue(1, 12);
			card.maxValue = card.minValue;
			card.textureId = TextureId::Coin;
			break;
		}
		case (CardType::WEAPON): {
			card = CardGenerator::GetRandomWeapon();
			break;
		}
		case (CardType::SHIELD): {
			card.name = "SHIELD";
			card.hp = GetRandomValue(2, 9);
			card.textureId = TextureId::ItemShield;
			break;
		}
		case (CardType::CHEST): {
			card.hp = 1;
			card.durability = 1;
			card.maxDurability = 1;
			card.name = "CHEST";
			card.textureId = TextureId::ItemChest;
			break;
		}
		case (CardType::KEY): {
			card.hp = 1;
			card.durability = 1;
			card.maxDurability = 1;
			card.name = "KEY";
			card.textureId = TextureId::ItemKey;
			break;
		}
		case (CardType::WEAPON_UPGRADE): {
			if (card.element == Element::FIRE) {
				card.name = "FIRE ELIXIR";
				card.maxValue = -1;
				card.minValue = -1;
				card.hp = 1;
				card.durability = -1;
				card.maxDurability = -1;
				card.textureId = TextureId::FlaskBigYellow;
				card.type = CardType::WEAPON_UPGRADE;
			} else if (card.element == Element::ICE) {
				card.name = "ICE ELIXIR";
				card.maxValue = -1;
				card.minValue = -1;
				card.hp = 1;
				card.durability = -1;
				card.maxDurability = -1;
				card.textureId = TextureId::FlaskBigBlue;
				card.type = CardType::WEAPON_UPGRADE;
			} else if (card.element == Element::ANVIL) {
				card.name = "ANVIL";
				card.type = CardType::WEAPON_UPGRADE;
				card.element = Element::ANVIL;
				card.textureId = TextureId::ItemAnvil;
				card.maxDurability = 2;
				card.durability = 2;
				card.hp = -1;
				card.maxValue = 5;
				card.minValue = 1;
			}
			break;
		}
		case (CardType::WAND): {
			card = CardGenerator::GetItemParms(tmp.type, tmp.element);

			card.maxDurability = 5;
			card.durability = 5;
			card.maxValue = 8;
			card.minValue = 2;
			break;
		}
		}

		masterDeck.push_back(card);
	}
	Card card;
	card.name = "ANVIL";
	card.type = CardType::WEAPON_UPGRADE;
	card.element = Element::ANVIL;
	card.textureId = TextureId::ItemAnvil;
	card.maxDurability = 2;
	card.durability = 2;
	card.hp = -1;
	card.maxValue = 5;
	card.minValue = 1;
	dungeonPiles[D_THREE].cards.push_back(card);

	Card sword;
	sword.name = "SWORD";
	sword.description = "SWORD - STANDARD WEAPON";
	sword.minValue = 3;
	sword.maxValue = 7;
	sword.durability = 5;
	sword.maxDurability = 5;
	sword.type = CardType::WEAPON;
	sword.element = Element::NONE;
	sword.textureId = TextureId::WeaponSword;

	dungeonPiles[D_ONE].cards.push_back(sword);

	Card player;
	player.name = "PLAYER";
	player.description = "YOUR HERO";
	player.hp = 13;
	player.maxValue = 13;
	player.minValue = -1;
	player.type = CardType::PLAYER;
	player.element = Element::NONE;
	player.textureId = TextureId::Knight1;

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
