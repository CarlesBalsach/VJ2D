#include "cTexture.h"

class Hadouken
{
public:
	Hadouken(int nx, int ny, int nvx);
	~Hadouken();
	void draw();
	void logic();
private:
	cTexture tex;
	int x,y;
	int w,h;
	int vx;
};