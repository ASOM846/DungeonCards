#pragma once

#include "game.hpp"
#include "menu.hpp"
#include "tutorial.hpp"

enum class WindowState {
	GAME,
	TUTORIAL,
	MENU,
};

class WindowManager {
  public:
	WindowManager() = default;
	~WindowManager() = default;

	void Init();

	void Run();

	void Update();
	void Render();

	void SwitchState(const WindowState newState);

  private:
	WindowState windowState;

	const int screenWidth = 1200;
	const int screenHeight = 800;
	const char *windowTitle = "DungeonCards";
	const int targetFPS = 60;

	Game game;
	Menu menu;
	TutorialState tutorial;
};
