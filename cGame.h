#pragma once

#include "SceneLoader.h"
#include "cPlayer.h"
#include "Monster1.h"
#include "cData.h"

#define GAME_WIDTH	640
#define GAME_HEIGHT 480

class cGame
{
public:
	cGame(void);
	virtual ~cGame(void);

	bool Init();
	bool Loop(float dt);
	void Finalize();

	//Input
	void ReadKeyboard(unsigned char key, int x, int y, bool press);
	void ReadMouse(int button, int state, int x, int y);
	//Process
	bool Process(float dt);
	//Output
	void Render();

private:
	static const int NUM_MONSTERS = 8;

	bool forward;
	void drawBackInTime();

	void nextLevel ();

	unsigned char keys[256];
	SceneLoader sceneLoader;
	cPlayer Player1;
	cPlayer Player2;
	std::vector<Monster1> monsters1;
	cData Data;

	float lastTick;
};
