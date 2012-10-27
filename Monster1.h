#pragma once

#include "cbicho.h"

class Monster1 : public cBicho
{
	enum Direction { left, right };
	Direction dir;

public:
	Monster1(void);
	~Monster1(void);

	void Randomise ();

	void Logic(int *map, bool forward);

	void Draw (int tex_id);
};
