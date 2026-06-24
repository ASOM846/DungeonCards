#include "windowManager.hpp"
#include "menu.hpp"
#include <raylib.h>

void WindowManager::Init() {
	InitWindow(screenWidth, screenHeight, windowTitle);

	SetWindowState(FLAG_WINDOW_RESIZABLE);

	SetTargetFPS(targetFPS);

	windowState = WindowState::MENU;

	game.Init();
	menu.Init();
}

void WindowManager::Run() {
	while (!WindowShouldClose()) {
		Update();
		Render();
	}

	CloseWindow();
}

void WindowManager::Update() {
	switch (windowState) {
	case WindowState::MENU: {
		MenuUpdateResoult resoult = menu.Update();
		if (resoult == MenuUpdateResoult::PLAY)
			SwitchState(WindowState::GAME);
		break;
	}
	case WindowState::GAME: {
		game.Update();

		if (game.shoudldReturnToMenu()) {
			SwitchState(WindowState::MENU);
		}
		break;
	}
	}
}

void WindowManager::Render() {
	BeginDrawing();
	ClearBackground(BLACK);

	Ui::DrawBackground(game.GetTextureManager()->get(TextureId::Background));
	Ui::DrawBackground(game.GetTextureManager()->get(TextureId::Vinete));

	switch (windowState) {
	case WindowState::MENU:
		menu.Draw(*game.GetTextureManager());
		break;
	case WindowState::GAME:
		game.Draw();
		break;
	}

	EndDrawing();
}

void WindowManager::SwitchState(const WindowState newState) {
	if (windowState == newState) {
		return;
	}

	switch (newState) {
	case WindowState::MENU:
		break;
	case WindowState::GAME:
		game.Reset();
		break;
	}

	windowState = newState;
}
