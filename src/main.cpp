#include "game.hpp"
#include "raylib.h"
#include "types.hpp"
#include <vector>

int main() {
	const int screenWidth = 1200;
	const int screenHeight = 800;

	InitWindow(screenWidth, screenHeight,
			   "raylib [core] example - basic window");

	SetTargetFPS(60);

	Game game;

	game.Init();

	while (!WindowShouldClose()) {
		game.Update();

		BeginDrawing();

		ClearBackground(LIGHTGRAY);
		game.Draw();

		EndDrawing();
	}

	CloseWindow();
	return 0;
}
