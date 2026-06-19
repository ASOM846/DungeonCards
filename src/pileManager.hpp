#pragma once

#include "pile.hpp"
#include "textureManager.hpp"
#include "types.hpp"
#include <raylib.h>

struct ItemParms {
	TextureId textureId;
	const char *name;
};

class PileManager {
  public:
	PileManager() = default;
	~PileManager() = default;

	void Reset();
	void Init();

	void UpdatePiles();
	void DrawAll(TextureManager &tm, Pile *selectedPile);

	void UpdatePilesOffset();

	Card *GetCardAt(Vector2 mousePos);

	Pile *GetPileAt(Vector2 mousePos);

	void RefillRoomIfNeeded();

	Pile &GetPlayerPile(PlayerSlot slot) { return playerPiles[slot]; }
	Pile &GetDungeonPile(DungeonSlot slot) { return dungeonPiles[slot]; }

	Pile *GetDungeonPiles() { return dungeonPiles; }

	TextureId GetRandomMonsterTexture();
	Card GetRandomSpell();
	ItemParms GetRandomWeaponParms();
	Card GenerateCardData(const CardType type, const Element element);

	[[nodiscard]] int GetMasterDeckSize() { return masterDeck.size(); }
	[[nodiscard]] std::vector<Card> &GetMasterDeck() { return masterDeck; }

  private:
	std::vector<Card> masterDeck;

	const int MAX_CARDS_ON_TABLE = 4;

	Pile playerPiles[P_COUNT];
	Pile dungeonPiles[D_COUNT];
	Pile discardPile[1];

	int pileWidth = 150;
	int pileHeight = 250;
};
