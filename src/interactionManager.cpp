#include "interactionManager.hpp"
#include "floatingText.hpp"
#include "screenShake.hpp"
#include "types.hpp"
#include <raylib.h>
#include <string>

bool InteractionManager::ShouldHighlight(Pile *selected, Pile *target) {
	if (selected == nullptr || selected == target) {
		return false;
	}
	if (selected->IsEmpty()) {
		return false;
	}

	CardType selType = selected->Back().type;

	if (target->IsEmpty()) {
		if (target->isLeftHand || target->isRightHand || target->isBackpack) {
			return (selType == CardType::WEAPON || selType == CardType::WAND ||
					selType == CardType::POTION ||
					selType == CardType::SHIELD || selType == CardType::SPELL);
		}
		return false;
	}

	CardType tarType = target->Back().type;

	if (selected->isBackpack) {
		return (target->isRightHand || target->isLeftHand);
	}

	if (selected->isDungeonPile && target->isDungeonPile)
		return false;

	if (selType == CardType::ENEMY) {
		return (tarType == CardType::PLAYER || tarType == CardType::SHIELD);
	}

	if ((selType == CardType::WEAPON && !selected->isBackpack) ||
		selType == CardType::WAND) {
		return (tarType == CardType::ENEMY);
	}

	if (selType == CardType::POTION) {
		return (tarType == CardType::PLAYER);
	}

	if (selType == CardType::COIN) {
		return (tarType == CardType::PLAYER);
	}

	Element selELement = selected->Back().element;

	if (selType == CardType::SPELL) {
		return (tarType == CardType::ENEMY);
	}
	return false;
}

void InteractionManager::Handle(Pile *&selected, Pile *target,
								InteractionContext &ctx) {
	if (selected == nullptr) {
		if (!target->cards.empty() && !target->isDiscardPile) {
			selected = target;
		}
		return;
	}

	if (selected == target) {
		selected = nullptr;
		return;
	}

	if (selected->isDungeonPile && target->isDungeonPile) {
		selected = nullptr;
		return;
	}

	if (target->isDiscardPile) {
		ResolveCardVsDiscardPile(selected, target, ctx);
		selected = nullptr;
		return;
	}

	if (target->cards.empty()) {
		if (target->isDungeonPile) {
			selected = nullptr;
			return;
		}

		if (selected->cards.back().type == CardType::ENEMY) {
			selected = nullptr;
			return;
		}

		HandleEmptyTargetMove(selected, target);
		selected = nullptr;
		return;
	}

	ResolveCardInteraction(selected, target, ctx);
	selected = nullptr;
}

void InteractionManager::HandleEmptyTargetMove(Pile *selected, Pile *target) {
	Card &sel = selected->cards.back();

	if (sel.type == CardType::PLAYER) {
		selected = nullptr;
		return;
	}

	target->cards.push_back(selected->cards.back());
	selected->cards.pop_back();
}

void InteractionManager::ResolveCardInteraction(Pile *selected, Pile *target,
												InteractionContext &ctx) {
	Card &sel = selected->cards.back();
	Card &tar = target->cards.back();

	if (sel.type == CardType::WEAPON && tar.type == CardType::ENEMY) {
		ResolveWeaponVsEnemy(selected, target, sel, tar, ctx);
	} else if (sel.type == CardType::ENEMY && tar.type == CardType::PLAYER) {
		ResolvePlayerVsEnemy(selected, tar, sel, ctx);
	} else if (sel.type == CardType::ENEMY && tar.type == CardType::SHIELD) {
		ResolveEnemyVsShield(selected, target, sel, tar, ctx);
	} else if (sel.type == CardType::POTION && tar.type == CardType::PLAYER) {
		ResolvePotionVsPlayer(selected, target, sel, tar, ctx);
	} else if (sel.type == CardType::WAND && tar.type == CardType::ENEMY) {
		ResolveWandVsEnemy(selected, target, sel, tar, ctx);
	} else if (sel.type == CardType::COIN && tar.type == CardType::PLAYER) {
		ResolveCoinVsPlayer(selected, target, sel, tar, ctx);
	} else if (sel.type == CardType::SPELL && tar.type == CardType::ENEMY) {
		ResolveSpellVsEnemy(selected, target, sel, tar, ctx);
	} else {
	}
}

void InteractionManager::ResolveCardVsDiscardPile(Pile *selected, Pile *target,
												  InteractionContext &ctx) {
	Card &topCard = selected->Back();

	if (selected->Back().type == CardType::PLAYER ||
		selected->Back().type == CardType::ENEMY) {
		return;
	}

	ctx.score += selected->Back().GetRandomVal() * selected->Back().durability;
	selected->cards.pop_back();
	ctx.cardsDefeated++;
}

void InteractionManager::ResolveWeaponVsEnemy(Pile *selected, Pile *target,
											  Card &sel, Card &tar,
											  InteractionContext &ctx) {

	if (selected->isBackpack)
		return;

	int weaponDamage = sel.GetRandomVal();
	int &zombieHp = tar.hp;

	ctx.effectManager.SpawnText(ctx.mousePos,
								"-" + std::to_string(weaponDamage), RED);
	if (weaponDamage >= zombieHp) {
		ctx.score += zombieHp;

		target->cards.pop_back();
		ctx.cardsDefeated++;

	} else {
		zombieHp -= weaponDamage;

		ctx.cardsDefeated++;
	}

	sel.durability--;

	if (sel.durability <= 0) {
		selected->cards.pop_back();
	}
}

void InteractionManager::ResolvePlayerVsEnemy(Pile *target, Card &sel,
											  Card &tar,
											  InteractionContext &ctx) {
	int &playerHp = sel.hp;
	int &enemyHp = tar.hp;

	ctx.effectManager.SpawnText(ctx.mousePos, "-" + std::to_string(enemyHp),
								RED);
	ctx.score += enemyHp;

	playerHp -= enemyHp;
	target->cards.pop_back();
	ctx.cardsDefeated++;
}

void InteractionManager::ResolveEnemyVsShield(Pile *selected, Pile *target,
											  Card &sel, Card &tar,
											  InteractionContext &ctx) {
	int &shieldVal = tar.hp;
	int &enemyHp = sel.hp;

	ctx.score += enemyHp;

	ctx.effectManager.SpawnText(ctx.mousePos, "-" + std::to_string(shieldVal),
								RED);

	if (shieldVal >= enemyHp) {
		shieldVal -= enemyHp;
		selected->cards.pop_back();
		ctx.cardsDefeated++;
		if (shieldVal <= 0) {
			target->cards.pop_back();
			ctx.cardsDefeated++;
		}
	} else if (enemyHp > shieldVal) {
		int remainingDmg = enemyHp - shieldVal;
		target->cards.pop_back();

		if ((ctx.playerPile != nullptr) && !ctx.playerPile->IsEmpty()) {

			ctx.playerPile->Back().hp -= remainingDmg;

			ctx.effectManager.SpawnText(ctx.playerPile->GetFloatingTextPos(),
										"-" + std::to_string(remainingDmg),
										RED);
		}

		selected->cards.pop_back();
		ctx.cardsDefeated++;
	}
}

void InteractionManager::ResolvePotionVsPlayer(Pile *selected, Pile *target,
											   Card &sel, Card &tar,
											   InteractionContext &ctx) {
	int potionVal = sel.GetRandomVal();
	tar.IncreaseHp(potionVal);

	ctx.effectManager.SpawnText(ctx.mousePos, "+" + std::to_string(potionVal),
								GREEN);
	selected->cards.pop_back();
	ctx.cardsDefeated++;
}

void InteractionManager::ResolveWandVsEnemy(Pile *selected, Pile *target,
											Card &sel, Card &tar,
											InteractionContext &ctx) {
	if (sel.element == tar.element) {
		return;
	}

	int dmgMultiplier = 1;
	Color textColor;

	textColor = sel.element == Element::ICE ? BLUE : ORANGE;

	if (((sel.element == Element::ICE) && (tar.element == Element::FIRE)) ||
		((sel.element == Element::FIRE) && (tar.element == Element::ICE))) {

		dmgMultiplier = 2;
	}

	int baseWandDamage = sel.GetRandomVal();
	int totalDamage = dmgMultiplier * baseWandDamage;
	int &tarHp = tar.hp;

	ctx.effectManager.SpawnText(ctx.mousePos, "-" + std::to_string(totalDamage),
								textColor);
	if (totalDamage >= tarHp) {
		ctx.score += tarHp;
		target->cards.pop_back();
		ctx.cardsDefeated++;
	} else {
		tar.hp -= totalDamage;
	}

	sel.durability--;

	if (sel.durability <= 0) {
		selected->cards.pop_back();
		ctx.cardsDefeated++;
	}
}

void InteractionManager::ResolveCoinVsPlayer(Pile *selected, Pile *target,
											 Card &sel, Card &tar,
											 InteractionContext &ctx) {
	ctx.score += sel.GetRandomVal();
	selected->cards.pop_back();
	ctx.cardsDefeated++;
}

void InteractionManager::ResolveSpellVsEnemy(Pile *selected, Pile *target,
											 Card &sel, Card &tar,
											 InteractionContext &ctx) {

	int selPower = sel.GetRandomVal();
	int &enemyHp = tar.hp;

	switch (sel.element) {
	case Element::LIFESTEAL: {
		int dmgDealt = 0;
		if (selPower >= enemyHp) {
			dmgDealt = enemyHp;
			ctx.score += enemyHp;

			target->cards.pop_back();
			ctx.cardsDefeated++;

		} else {
			dmgDealt = selPower;
			enemyHp -= selPower;
			ctx.score += selPower;
		}

		selected->cards.pop_back();
		ctx.cardsDefeated++;

		if (ctx.playerPile != nullptr && !ctx.playerPile->IsEmpty()) {
			ctx.playerPile->Back().IncreaseHp(dmgDealt);
		}
		break;
	}
	case Element::WARHAMMER: {
		ctx.masterDeck.insert(ctx.masterDeck.begin(), tar);
		target->cards.pop_back();
		selected->cards.pop_back();
		ctx.cardsDefeated++;
		return;
	}
	case Element::ESCAPE: {

		if (selected != nullptr && !selected->IsEmpty()) {
			selected->cards.pop_back();
		}
		for (int i = 0; i < D_COUNT; i++) {
			while (!ctx.dungeonPiles[i].IsEmpty()) {
				ctx.masterDeck.insert(ctx.masterDeck.begin(),
									  ctx.dungeonPiles[i].Back());
				ctx.dungeonPiles[i].cards.pop_back();
			}
		}

		for (int i = 0; i < D_COUNT; i++) {
			if (!ctx.masterDeck.empty()) {
				ctx.dungeonPiles[i].cards.push_back(ctx.masterDeck.back());
				ctx.masterDeck.pop_back();
			}
		}
		return;
	}
	}
}
