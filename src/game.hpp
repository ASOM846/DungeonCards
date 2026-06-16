#pragma once

#include "pileManager.hpp"
#include "ui.hpp"
#include <raylib.h>

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
	PileManager pileManager;
	Ui ui;

	int score;
	bool gameOver;

	Pile *selected = nullptr;
};
