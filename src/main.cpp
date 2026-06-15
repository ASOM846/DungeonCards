#include "raylib.h"
#include <string>
#include <vector>

struct Card {
	std::string name;
	int value;
	int type; // 0 - bron 1 - potka 2 - przeciwnik
};

struct Pile {
	std::vector<Card> cards;
	Vector2 position;
};

enum PlayerSlot {
	P_LEFT,
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

	for (int i = 0; i < D_COUNT; i++) {
		state.dungeonPiles[i].position = {
			.x = (float)(spacing + i * (pileWidth + spacing)), .y = 20.0f};
	}

	for (int i = 0; i < P_COUNT; i++) {
		state.playerPiles[i].position = {
			.x = (float)(spacing + i * (pileWidth + spacing)), .y = 300.0f};
	}

	Card card1 = {.name = "HP", .value = 5, .type = 1};
	Card card2 = {.name = "monster", .value = 3, .type = 2};
	Card card3 = {.name = "miecz", .value = 2, .type = 0};
	Card card4 = {.name = "monster", .value = 3, .type = 2};

	state.dungeonPiles[D_ONE].cards.push_back(card1);
	state.dungeonPiles[D_TWO].cards.push_back(card2);
	state.dungeonPiles[D_THREE].cards.push_back(card3);

	state.playerPiles[P_LEFT].cards.push_back(card3);
	state.playerPiles[P_RIGHT].cards.push_back(card3);

	Pile *selectedPile;

	while (!WindowShouldClose()) {
		Vector2 mousePos = GetMousePosition();

		for (auto &dungeonPile : state.dungeonPiles) {
			Rectangle rec = {dungeonPile.position.x, dungeonPile.position.y,
							 (float)pileWidth, (float)pileHeight};
			if (CheckCollisionPointRec(mousePos, rec) &&
				IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
				if (selectedPile == &dungeonPile) {
					selectedPile = nullptr;
				} else {
					selectedPile = &dungeonPile;
				}
			}
		}

		for (auto &playerPile : state.playerPiles) {
			Rectangle rec = {playerPile.position.x, playerPile.position.y,
							 (float)pileWidth, (float)pileHeight};
			if (CheckCollisionPointRec(mousePos, rec) &&
				IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
				if (selectedPile == &playerPile) {
					selectedPile = nullptr;
				} else {
					selectedPile = &playerPile;
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
