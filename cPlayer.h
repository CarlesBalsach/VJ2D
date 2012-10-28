#pragma once

#include "cBicho.h"
#include "Hadouken.h"
#include <vector>
#include "Monster1.h"

#define STATE_SHOOTING_LEFT 4
#define STATE_SHOOTING_RIGHT 5

class cPlayer: public cBicho
{
public:
	cPlayer();
	~cPlayer();
	void Logic(int *map, bool forward);
	void Draw(int tex_id, bool forward);
	void addInput(int input);
	
	//overriding to avoid animation states override
	void MoveRight(int *map);
	void MoveLeft(int *map);
	void Jump(int *map);
	void StopJumping(int *map);
	void Stop();

	void MonstersCollisions(vector<Monster1>& monsters1);

private:
	void addHadouken();
	vector<Hadouken> hadoukens;
	vector<int> inputs;
	int shootAnimationFrame;
};
