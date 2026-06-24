#pragma once

#include "floatingText.hpp"
#include "pile.hpp"
#include "screenShake.hpp"
#include <vector>

enum class IteractionResoult {
	NONE,
	WEAPON_HIT,
	WEAPON_KILL,
	PLAYER_HIT,
};

struct InteractionContext {
	int &score;
	int &cardsDefeated;
	Vector2 mousePos;
	Pile *playerPile;
	Pile *dungeonPiles;
	std::vector<Card> &masterDeck;
	ScreenShake &screenShake;
	EffectManager &effectManager;
};

class InteractionManager {
  public:
	static bool ShouldHighlight(Pile *selected, Pile *target);

	static void Handle(Pile *&selected, Pile *target, InteractionContext &ctx);

  private:
	static void HandleEmptyTargetMove(Pile *selected, Pile *target);
	static void ResolveCardInteraction(Pile *selected, Pile *target,
									   InteractionContext &ctx);

	static void ResolveCardVsDiscardPile(Pile *selected, Pile *target,
										 InteractionContext &ctx);

	static void ResolveWeaponVsEnemy(Pile *selected, Pile *target, Card &sel,
									 Card &tar, InteractionContext &ctx);
	static void ResolvePlayerVsEnemy(Pile *target, Card &sel, Card &tar,
									 InteractionContext &ctx);
	static void ResolveEnemyVsShield(Pile *selected, Pile *target, Card &sel,
									 Card &tar, InteractionContext &ctx);
	static void ResolvePotionVsPlayer(Pile *selected, Pile *target, Card &sel,
									  Card &tar, InteractionContext &ctx);
	static void ResolveWandVsEnemy(Pile *selected, Pile *target, Card &sel,
								   Card &tar, InteractionContext &ctx);
	static void ResolveCoinVsPlayer(Pile *selected, Pile *target, Card &sel,
									Card &tar, InteractionContext &ctx);

	static void ResolveSpellVsEnemy(Pile *selected, Pile *target, Card &sel,
									Card &tar, InteractionContext &ctx);
};
