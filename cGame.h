#pragma once

#include "SceneLoader.h"
#include "cPlayer.h"
#include "Monster1.h"
#include "Monster2.h"
#include "cData.h"
#include "Menu.h"

#define GAME_WIDTH	640
#define GAME_HEIGHT 480

class cGame
{
public:
	cGame(void);
	virtual ~cGame(void);

	bool Init();
	void Reset();
	bool Loop(float dt);
	void Finalize();

	//Input
	void ReadKeyboard(unsigned char key, int x, int y, bool press);
	void ReadMouse(int button, int state, int x, int y);
	void MoveMouse(int x, int y);
	//Process
	bool Process(float dt);
	//Output
	void Render();

private:
	static const int NUM_MONSTERS  = 5;
	static const int NUM_MONSTERS2 = 2;
	static const int NUM_LEVELS = 3;

	enum Status
	{ Menu
	, Playing
	, Help
	, Credits
	};

	bool forward;
	void drawBackInTime();

	void nextLevel ();
	void setStatus (Status status);
	void translateMouse (float& x, float& y);

	unsigned char keys[256];
	bool buttons[8];
	int xmouse, ymouse;

	SceneLoader sceneLoader;
	cPlayer Player1;
	cPlayer Player2;
	std::vector<Monster1> monsters1;
	Monster2 monsters2[NUM_MONSTERS2];
	cData Data;
	::Menu* menu;

	Status status;

	float lastTick;
	float deathTime;
	float gameOverElapsed;
	float youWinElapsed;
	bool invulnerable;
	bool isGameOver;

	cTexture heart;
	cTexture bullet;
	cTexture turret;
	cTexture gameOver;
	cTexture youWin;
	cTexture flowers;
};
