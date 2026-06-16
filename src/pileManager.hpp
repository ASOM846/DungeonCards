#pragma once

#include "pile.hpp"
#include "types.hpp"

class PileManager {
  public:
	PileManager() = default;
	~PileManager() = default;

	void Reset();
	void Init();

	void DrawAll(const Pile *selectedPile);
	Pile *GetPileAt(Vector2 mousePos);

	Pile &GetPlayerPile(PlayerSlot slot) { return playerPiles[slot]; }
	Pile &GetDungeonPile(DungeonSlot slot) { return dungeonPiles[slot]; }

	Card GenerateRandomCard();
	Card GenerateCard(const CardType type, const Element element);

  private:
	Pile playerPiles[P_COUNT];
	Pile dungeonPiles[D_COUNT];

	int pileWidth = 150;
	int pileHeight = 250;
};
