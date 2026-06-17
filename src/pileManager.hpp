#pragma once

#include "pile.hpp"
#include "types.hpp"

class PileManager {
  public:
	PileManager() = default;
	~PileManager() = default;

	void Reset();
	void Init();

	void DrawAll(Pile *selectedPile);
	Pile *GetPileAt(Vector2 mousePos);

	void RefillRoomIfNeeded();

	Pile &GetPlayerPile(PlayerSlot slot) { return playerPiles[slot]; }
	Pile &GetDungeonPile(DungeonSlot slot) { return dungeonPiles[slot]; }

	Card GenerateRandomCard();
	Card GenerateCard(const CardType type, const Element element);

	[[nodiscard]] int GetMasterDeckSize() { return masterDeck.size(); }

  private:
	std::vector<Card> masterDeck;

	const int MAX_CARDS_ON_TABLE = 4;

	Pile playerPiles[P_COUNT];
	Pile dungeonPiles[D_COUNT];
	Pile discardPile[1];

	int pileWidth = 150;
	int pileHeight = 250;
};
