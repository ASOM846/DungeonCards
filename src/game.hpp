#pragma once
#include "button.hpp"
#include "pileManager.hpp"
#include "textureManager.hpp"
#include "ui.hpp"
#include <raylib.h>

enum class GameState {
	PLAYING,
	WIN,
	LOSE,
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

	TextureManager *GetTextureManager() { return &textureManager; }

	bool shoudldReturnToMenu() const { return shouldReturnToMenu; }

  private:
	TextureManager textureManager;

	PileManager pileManager;
	Ui ui;

	GameState gameState;

	int score;

	Button goBackButton;

	Pile *selected = nullptr;

	void DrawLose();
	void DrawWin();

	bool shouldReturnToMenu;
	int cardsDefeated;
};
