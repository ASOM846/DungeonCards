#pragma once

#include "game.hpp"
class WindowManager {
  public:
	WindowManager() = default;
	~WindowManager() = default;

	void Init();

	void Run();

	void Update();
	void Render();

  private:
	const int screenWidth = 1200;
	const int screenHeight = 800;
	const char *windowTitle = "DungeonCards";
	const int targetFPS = 60;

	Game game;
};
