#pragma once

#include "pile.hpp"
#include "textureManager.hpp"
#include "types.hpp"
#include <raylib.h>

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

	[[nodiscard]] int GetMasterDeckSize() { return masterDeck.size(); }
	[[nodiscard]] int GetMasterDeckStartingSize() const { return cardsInDeck; }
	[[nodiscard]] std::vector<Card> &GetMasterDeck() { return masterDeck; }

  private:
	std::vector<Card> masterDeck;

	const int MAX_CARDS_ON_TABLE = 4;

	int cardsInDeck = 0;

	Pile playerPiles[P_COUNT];
	Pile dungeonPiles[D_COUNT];
	Pile discardPile[1];

	int pileWidth = 150;
	int pileHeight = 250;
};
