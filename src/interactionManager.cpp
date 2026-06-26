#include "interactionManager.hpp"
#include "floatingText.hpp"
#include "screenShake.hpp"
#include "textureManager.hpp"
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
					selType == CardType::POTION || selType == CardType::KEY ||
					selType == CardType::SHIELD || selType == CardType::SPELL);
		}
		return false;
	}

	CardType tarType = target->Back().type;

	bool isSelItemSlot =
		selected->isLeftHand || selected->isRightHand || selected->isBackpack;
	bool isTarItemSlot =
		target->isLeftHand || target->isRightHand || target->isBackpack;

	if (isSelItemSlot && isTarItemSlot) {
		return true;
	}

	if (selected->isDungeonPile && target->isDungeonPile) {
		return false;
	}

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

	if (selType == CardType::KEY) {
		return (tarType == CardType::CHEST);
	}

	if (selType == CardType::WEAPON_UPGRADE) {
		return (tarType == CardType::WEAPON);
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

		if (selected->cards.back().type == CardType::ENEMY ||
			selected->Back().type == CardType::CHEST) {
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

	bool isSelItemSlot =
		selected->isLeftHand || selected->isRightHand || selected->isBackpack;
	bool isTarItemSlot =
		target->isLeftHand || target->isRightHand || target->isBackpack;

	if (isSelItemSlot && isTarItemSlot) {
		if (!(sel.type == CardType::WEAPON_UPGRADE)) {

			std::swap(selected->Back(), target->Back());
			return;
		}
	}

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
	} else if (sel.type == CardType::KEY && tar.type == CardType::CHEST) {
		ResolveKeyVsChest(selected, target, sel, tar, ctx);
	} else if (sel.type == CardType::WEAPON_UPGRADE &&
			   tar.type == CardType::WEAPON) {
		ResolveWeaponUpgradeVsWeapon(selected, target, sel, tar, ctx);
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
	ctx.screenShake.trigger();

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
	ctx.screenShake.trigger();

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
	ctx.screenShake.trigger();

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
	ctx.screenShake.trigger();

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
	int coinVal = sel.GetRandomVal();
	ctx.score += coinVal;
	ctx.effectManager.SpawnText(ctx.mousePos, "+" + std::to_string(coinVal),
								GOLD);

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
		ctx.screenShake.trigger();
		ctx.effectManager.SpawnText(ctx.mousePos,
									"-" + std::to_string(dmgDealt), RED);

		if (ctx.playerPile != nullptr && !ctx.playerPile->IsEmpty()) {
			ctx.playerPile->Back().IncreaseHp(dmgDealt);
		}
		break;
	}
	case Element::WARHAMMER: {
		ctx.masterDeck.insert(ctx.masterDeck.begin(), tar);
		ctx.screenShake.trigger();

		target->cards.pop_back();
		ctx.cardsDefeated++;

		selected->cards.pop_back();
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

	sel.durability--;

	if (sel.durability <= 0) {
		selected->cards.pop_back();
		ctx.cardsDefeated++;
	}
}

void InteractionManager::ResolveKeyVsChest(Pile *selected, Pile *target,
										   Card &sel, Card &tar,
										   InteractionContext &ctx) {
	selected->cards.pop_back();

	tar.type = CardType::COIN;
	tar.name = "COIN";
	tar.textureId = TextureId::Coin;

	tar.minValue = GetRandomValue(30, 50);
	tar.maxValue = 50;

	tar.hp = -1;
	tar.durability = -1;
	tar.maxDurability = -1;

	ctx.effectManager.SpawnText(ctx.mousePos, "OPEN!", GOLD);
	ctx.screenShake.trigger();
}

void InteractionManager::ResolveWeaponUpgradeVsWeapon(Pile *selected,
													  Pile *target, Card &sel,
													  Card &tar,
													  InteractionContext &ctx) {
	switch (sel.element) {
	case Element::NONE:
	case Element::FIRE: {
		if (tar.type == CardType::WEAPON) {
			tar.name = "FIRE " + tar.name;
		}

		tar.maxValue += 2;
		tar.minValue += 2;

		tar.element = Element::FIRE;

		ctx.effectManager.SpawnText(ctx.mousePos, "+2", ORANGE);
		selected->cards.pop_back();
		return;
	}
	case Element::ICE: {
		if (tar.type == CardType::WEAPON) {
			tar.name = "ICE " + tar.name;
		}

		tar.maxValue += 3;
		tar.minValue += 3;

		tar.element = Element::ICE;
		ctx.effectManager.SpawnText(ctx.mousePos, "+3", BLUE);
		selected->cards.pop_back();
		return;
	}
	case Element::ANVIL: {
		int repairVal = sel.GetRandomVal();
		tar.Repair(repairVal);

		sel.durability--;
		if (sel.durability <= 0) {
			selected->cards.pop_back();
		}
		return;
	}
	case Element::LIFESTEAL:
	case Element::WARHAMMER:
	case Element::ESCAPE:
	case Element::SACRIFICE:
	case Element::COUNT:
		break;
	}
}
