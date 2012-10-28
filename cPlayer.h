#pragma once

#include "cBicho.h"
#include "Hadouken.h"
#include <vector>
#include "Monster1.h"

class cPlayer: public cBicho
{
public:
	cPlayer();
	~cPlayer();
	void Logic(int *map, bool forward);
	void Draw(int tex_id, bool forward);
	void addInput(int input);
	void toSpawnZone(int tileX, int tileY);
	
	//overriding to avoid animation states override
	void MoveRight(int *map);
	void MoveLeft(int *map);
	void Jump(int *map);
	void StopJumping(int *map);
	void Stop();

	void MonstersCollisions(vector<Monster1>& monsters1);
	void setDead (bool val);
	bool isDead();
	void IA(int * map, cRect enemy);

	int getLives () const;

private:
	void addHadouken();
	vector<Hadouken> hadoukens;
	vector<int> inputs;
	int shootAnimationFrame;
	bool dead;
	int lives;
};
