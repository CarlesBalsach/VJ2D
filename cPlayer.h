#pragma once

#include "cBicho.h"
#include "Hadouken.h"
#include <vector>

#define PLAYER_START_CX		3
#define PLAYER_START_CY		2

class cPlayer: public cBicho
{
public:
	cPlayer();
	~cPlayer();
	void Logic(int *map, bool forward);
	void Draw(int tex_id, bool forward);
	void addInput(int input);
	
private:
	void addHadouken();
	vector<Hadouken> hadoukens;
	vector<int> inputs;
};
