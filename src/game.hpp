#pragma once
#include "pileManager.hpp"
#include "textureManager.hpp"
#include "ui.hpp"
#include <raylib.h>

enum class GameState {
	PLAYING,
	WIN,
	LOSE
};

class Game {
  public:
	Game() = default;
	~Game() = default;

	void Init();
	void Reset();

	void Run();

	void Update();
	void Draw();

  private:
	TextureManager textureManager;

	PileManager pileManager;
	Ui ui;

	GameState gameState;

	int score;

	Pile *selected = nullptr;

	void DrawLose();
	void DrawWin();
};
