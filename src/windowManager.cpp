#include "windowManager.hpp"
#include <raylib.h>

void WindowManager::Init() {
	InitWindow(screenWidth, screenHeight, windowTitle);

	SetTargetFPS(60);

	game.Init();
}

void WindowManager::Run() {
	while (!WindowShouldClose()) {
		Update();
		Render();
	}

	CloseWindow();
}

void WindowManager::Update() {
	game.Update();
}

void WindowManager::Render() {
	game.Draw();
}
