#include "Monster1.h"
#include "cScene.h"


Monster1::Monster1(void) : dir (left), alive (true)
{
}


Monster1::~Monster1(void)
{
}


void Monster1::Randomise ()
{
	int x = 10 + rand() % (SCENE_WIDTH-16);
	int y = 2+rand() % (SCENE_HEIGHT-10);
	SetTile (x, y);

	int d = rand() % 2;
	if (d == 0)
	{
		dir = left;
		SetState(STATE_WALKLEFT);
	}
	else
	{
		dir = right;
		SetState(STATE_WALKRIGHT);
	}
	alive = true;
	jumping = false;
}


void Monster1::Logic(int *map, bool forward)
{
	if (!alive) return;
	cBicho::Logic (map, forward);
	jumping = false;

	// Move from one end of the platform to the other.
	int old_x = x;
	int old_y = y;

	if (dir == left) MoveLeft(map);
	else             MoveRight(map);

	bool same_pos = x == old_x && y == old_y;

	if (!CollidesMapFloor(map) || same_pos)
	{
		if (dir == left)
		{
			if (!same_pos) MoveRight (map);
			SetState(STATE_WALKRIGHT);
			dir = right;
		}
		else
		{
			if (!same_pos) MoveLeft (map);
			SetState(STATE_WALKLEFT);
			dir = left;
		}
	}

	NextFrame(3);
}


void Monster1::Draw (int tex_id) const
{
	if (!alive) return;

	float xo, yo, xf, yf;
	switch(GetState())
	{
		case STATE_WALKLEFT:
			xo = 0.0f;	yo = 0.25f + (GetFrame()*0.25f);
			break;

		default:
			xo = 0.25f; yo = 0.25f + (GetFrame()*0.25f);
			break;
	}
	xf = xo + 0.25f;
	yf = yo - 0.25f;

	glColor3f (1.0f, 0.0f, 0.0f);
	DrawRect(tex_id,xo,yo,xf,yf);
	glColor3f (1.0f, 1.0f, 1.0f);
}


void Monster1::setAlive (bool val)
{
	alive = val;
}


bool Monster1::isAlive () const
{
	return alive;
}
