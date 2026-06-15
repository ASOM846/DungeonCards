#include "raylib.h"
#include <string>
#include <vector>

enum class CardType {
	ENEMY,
	WEAPON,
	POTION,
	PLAYER,
	COUNT
};

struct Card {
	std::string name;
	int value;
	CardType type;
};

struct Pile {
	std::vector<Card> cards;
	Vector2 position;
};

enum PlayerSlot {
	P_LEFT,
	P_PLAYER,
	P_RIGHT,
	P_BACKPACK,
	P_COUNT
};

enum DungeonSlot {
	D_ONE,
	D_TWO,
	D_THREE,
	D_COUNT
};

struct GameState {
	Pile playerPiles[P_COUNT];
	Pile dungeonPiles[D_COUNT];
};

Card GenerateCard(const CardType type) {
	Card c;
	c.type = type;

	switch (type) {
	case CardType::ENEMY:
		c.name = "MONSTER";
		c.value = GetRandomValue(1, 8);
		break;
	case CardType::WEAPON:
		c.name = "WEAPON";
		c.value = GetRandomValue(2, 5);
		break;
	case CardType::POTION:
		c.name = "POTION - HP";
		c.value = GetRandomValue(2, 6);
		break;
	case CardType::PLAYER:
	case CardType::COUNT:
		break;
	}
	return c;
}

Card GenerateRandomCard() {
	auto randomType = static_cast<CardType>(
		GetRandomValue(0, static_cast<int>(CardType::PLAYER) - 1));

	return GenerateCard(randomType);
}

void DrawPile(Pile &pile, int &width, int &height, bool isSelected) {
	DrawRectangleV(pile.position,
				   {static_cast<float>(width), static_cast<float>(height)},
				   GRAY);

	if (isSelected)
		DrawRectangleLinesEx(
			{pile.position.x, pile.position.y, (float)width, (float)height}, 4,
			ORANGE);

	if (pile.cards.empty()) {
		return;
	}

	Card *topCard = &pile.cards.back();

	DrawText(topCard->name.c_str(), pile.position.x + 10, pile.position.y + 10,
			 20, BLACK);
	DrawText(TextFormat("Val: %d", topCard->value), pile.position.x + 10,
			 pile.position.y + 40, 20, DARKBLUE);
}

void HandleInteraction(Pile *&selected, Pile &target) {
	if (selected == nullptr) {
		if (!target.cards.empty())
			selected = &target;
		return;
	}
	if (selected == &target || target.cards.empty()) {
		selected = nullptr;
		return;
	}

	Card &sel = selected->cards.back();
	Card &tar = target.cards.back();

	if (sel.type == CardType::WEAPON && tar.type == CardType::ENEMY) {
		if (sel.value >= tar.value) {
			sel.value -= tar.value;

			target.cards.pop_back();

			if (sel.value <= 0) {
				selected->cards.pop_back();
			}
		} else {
			tar.value -= sel.value;
			selected->cards.pop_back();
		}
	} else if (sel.type == CardType::POTION && tar.type == CardType::PLAYER) {
		tar.value += sel.value;
		selected->cards.pop_back();
	}

	selected = nullptr;
}

int main() {
	const int screenWidth = 800;
	const int screenHeight = 600;

	int pileWidth = 150;
	int pileHeight = 250;

	int hp = 10;

	InitWindow(screenWidth, screenHeight,
			   "raylib [core] example - basic window");

	SetTargetFPS(60);

	GameState state;

	int spacing = 20;

	int totalWidth = (D_COUNT * pileWidth) + ((D_COUNT - 1) * spacing);
	int startX = (screenWidth - totalWidth) / 2;

	for (int i = 0; i < D_COUNT; i++) {
		state.dungeonPiles[i].position = {
			.x = (float)(startX + i * (pileWidth + spacing)), .y = 20.0f};
	}

	totalWidth = (P_COUNT * pileWidth) + ((P_COUNT - 1) * spacing);
	startX = (screenWidth - totalWidth) / 2;

	for (int i = 0; i < P_COUNT; i++) {
		state.playerPiles[i].position = {
			.x = (float)(startX + i * (pileWidth + spacing)), .y = 300.0f};
	}

	Card player = {.name = "PLAYER", .value = 10, .type = CardType::PLAYER};

	for (auto &i : state.dungeonPiles) {
		for (int j = 0; j < 10; j++) {
			i.cards.push_back(GenerateCard(CardType::ENEMY));
		}
	}

	Card sword = {.name = "SWORD", .value = 8, .type = CardType::WEAPON};
	Card hpPotion = {.name = "HP POTION", .value = 4, .type = CardType::POTION};

	state.playerPiles[P_PLAYER].cards.push_back(player);
	state.playerPiles[P_RIGHT].cards.push_back(sword);
	state.playerPiles[P_BACKPACK].cards.push_back(hpPotion);

	Pile *selectedPile = nullptr;

	while (!WindowShouldClose()) {
		Vector2 mousePos = GetMousePosition();

		if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
			Vector2 mousePos = GetMousePosition();

			for (auto &dungeonPile : state.dungeonPiles) {
				if (CheckCollisionPointRec(mousePos, {dungeonPile.position.x,
													  dungeonPile.position.y,
													  (float)pileWidth,
													  (float)pileHeight})) {
					HandleInteraction(selectedPile, dungeonPile);
				}
			}

			for (auto &playerPile : state.playerPiles) {
				if (CheckCollisionPointRec(
						mousePos, {playerPile.position.x, playerPile.position.y,
								   (float)pileWidth, (float)pileHeight})) {
					HandleInteraction(selectedPile, playerPile);
				}
			}
		}

		BeginDrawing();

		ClearBackground(RAYWHITE);

		for (auto &pile : state.dungeonPiles) {
			bool isSel = (&pile == selectedPile);
			DrawPile(pile, pileWidth, pileHeight, isSel);
		}

		for (auto &pile : state.playerPiles) {
			bool isSel = (&pile == selectedPile);
			DrawPile(pile, pileWidth, pileHeight, isSel);
		}

		DrawText(TextFormat("HP: %d", hp), 40, GetScreenHeight() - 40, 20, RED);

		EndDrawing();
	}

	CloseWindow();
	return 0;
}
