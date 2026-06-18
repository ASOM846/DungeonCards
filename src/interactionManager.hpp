#pragma once

#include "pile.hpp"
class InteractionManager {
  public:
	static bool ShouldHighlight(Pile *selected, Pile *target);

	static void Handle(Pile *&selected, Pile *target, int &score,
					   Pile *playerPile, Pile *dungeonPiles,
					   std::vector<Card> &masterDeck);

  private:
	static void HandleEmptyTargetMove(Pile *selected, Pile *target);
	static void ResolveCardInteraction(Pile *selected, Pile *target, int &score,
									   Pile *playerPile,
									   std::vector<Card> &masterDeck,
									   Pile *dungeonPiles);

	static void ResolveCardVsDiscardPile(Pile *selected, Pile *target,
										 int &score);

	static void ResolveWeaponVsEnemy(Pile *selected, Pile *target, Card &sel,
									 Card &tar, int &score, Pile *playerPile,
									 std::vector<Card> &masterDeck);
	static void ResolvePlayerVsEnemy(Pile *target, Card &sel, Card &tar,
									 int &score);
	static void ResolveEnemyVsShield(Pile *selected, Pile *target, Card &sel,
									 Card &tar, int &score, Pile *playerPile);
	static void ResolvePotionVsPlayer(Pile *selected, Pile *target, Card &sel,
									  Card &tar, int &score);
	static void ResolveWandVsEnemy(Pile *selected, Pile *target, Card &sel,
								   Card &tar, int &score);
	static void ResolveCoinVsPlayer(Pile *selected, Pile *target, Card &sel,
									Card &tar, int &score);

	static void ResolveSpellVsEnemy(Pile *selected, Pile *target, Card &sel,
									Card &tar, int &score, Pile *playerPile,
									std::vector<Card> &masterDeck,
									Pile *dungeonPiles);
};
