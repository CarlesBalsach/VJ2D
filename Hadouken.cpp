#include "Hadouken.h"

Hadouken::Hadouken(int nx, int ny, int nvx)
{
	bool success = tex.Load("hadouken.png",GL_RGBA);
	x = nx;
	y = ny;
	w = 32;
	h = 32;
	vx = nvx;
}
Hadouken::~Hadouken()
{}

void Hadouken::draw()
{
	glEnable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D,tex.GetID());
	glBegin(GL_QUADS);
	if(vx<0) {
		glTexCoord2f(1,0); glVertex2i(x,y);
		glTexCoord2f(0,0); glVertex2i(x+w,y);
		glTexCoord2f(0,1); glVertex2i(x+w,y+h);
		glTexCoord2f(1,1); glVertex2i(x,y+h);
	}
	else
	{
		glTexCoord2f(0,0); glVertex2i(x,y);
		glTexCoord2f(1,0); glVertex2i(x+w,y);
		glTexCoord2f(1,1); glVertex2i(x+w,y+h);
		glTexCoord2f(0,1); glVertex2i(x,y+h);
	}
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void Hadouken::logic()
{
	x += vx;
}

cRect Hadouken::getArea()
{
	cRect rect;
	rect.left = x;
	rect.right = x+w;
	rect.bottom = y;
	rect.top = y+h;
	return rect;
}