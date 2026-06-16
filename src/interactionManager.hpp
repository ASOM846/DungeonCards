#pragma once

#include "pile.hpp"
class InteractionManager {
  public:
	static void Handle(Pile *&selected, Pile *target, int &score);

  private:
	static void HandleEmptyTargetMove(Pile *selected, Pile *target);
	static void ResolveCardInteraction(Pile *selected, Pile *target,
									   int &score);

	static void ResolveWeaponVsEnemy(Pile *selected, Pile *target, Card &sel,
									 Card &tar, int &score);
	static void ResolvePlayerVsEnemy(Pile *target, Card &sel, Card &tar,
									 int &score);
	static void ResolvePotionVsPlayer(Pile *selected, Pile *target, Card &sel,
									  Card &tar, int &score);
	static void ResolveWandVsEnemy(Pile *selected, Pile *target, Card &sel,
								   Card &tar, int &score);
};
