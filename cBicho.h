#pragma once

#include "cTexture.h"
#include "Globals.h"
#include <vector>
using namespace std;

#define FRAME_DELAY		8
#define STEP_LENGTH		2
#define JUMP_HEIGHT		8
#define JUMP_STEP		5

#define STATE_LOOKLEFT			0
#define STATE_LOOKRIGHT			1
#define STATE_WALKLEFT			2
#define STATE_WALKRIGHT			3
#define STATE_SHOOTING_LEFT		4
#define STATE_SHOOTING_RIGHT	5
#define STATE_DEAD				6

class cRect
{
public:
	int left,top,
		right,bottom;
};

class cBichoState
{
public:
	int x,y;
	int w,h;
	int state;
	bool jumping;
	int jump_alfa;
	int seq,delay;
};

class cBicho
{
public:
	cBicho(void);
	cBicho(int x,int y,int w,int h);
	~cBicho(void);

	void SetPosition(int x,int y);
	void GetPosition(int *x,int *y) const;
	void SetTile(int tx,int ty);
	void GetTile(int *tx,int *ty) const;
	void SetWidthHeight(int w,int h);
	void GetWidthHeight(int *w,int *h) const;

	bool Collides(cRect rc) const;
	bool CollidesMapWall(int *map,bool right) const;
	bool CollidesMapFloor(int *map);
	cRect GetArea() const;
	void DrawRect(int tex_id,float xo,float yo,float xf,float yf) const;

	void MoveRight(int *map);
	void MoveLeft(int *map);
	void Jump(int *map);
	void StopJumping(int *map);
	void Stop();
	void Logic(int *map, bool forward);

	int  GetState() const;
	void SetState(int s);

	void NextFrame(int max);
	int  GetFrame() const;
	
	//this state is related with going backwards in time
	void addState();
	cBichoState getBichoState() const;
	void setBichoState(cBichoState bState);
protected:
	vector<cBichoState> states;

	int x,y;
	int w,h;
	int state;

	bool jumping;
	int jump_alfa;

	int seq,delay;
};
