#include "cBicho.h"
#include "cScene.h"
#include "Globals.h"
#include <iostream>
using namespace std;

cBicho::cBicho(void)
{
	seq=0;
	delay=0;

	jumping = false;

	states = vector<cBichoState>(0);
}
cBicho::~cBicho(void){}

cBicho::cBicho(int posx,int posy,int width,int height)
{
	x = posx;
	y = posy;
	w = width;
	h = height;
	jumping = false;
}
void cBicho::SetPosition(int posx,int posy)
{
	x = posx;
	y = posy;
}
void cBicho::GetPosition(int *posx,int *posy) const
{
	*posx = x;
	*posy = y;
}
void cBicho::SetTile(int tx,int ty)
{
	x = tx * TILE_SIZE;
	y = ty * TILE_SIZE;
}
void cBicho::GetTile(int *tx,int *ty) const
{
	*tx = x / TILE_SIZE;
	*ty = y / TILE_SIZE;
}
void cBicho::SetWidthHeight(int width,int height)
{
	w = width;
	h = height;
}
void cBicho::GetWidthHeight(int *width,int *height) const
{
	*width = w;
	*height = h;
}
bool cBicho::Collides(cRect rc) const
{
	return !((x>rc.right) || (x+w<rc.left) || (y>rc.top) || (y+h<rc.bottom));
}
bool cBicho::CollidesMapWall(int *map,bool right) const
{
	int tile_x,tile_y;
	int j;
	int width_tiles,height_tiles;

	tile_x = x / TILE_SIZE;
	tile_y = y / TILE_SIZE;
	width_tiles  = w / TILE_SIZE;
	height_tiles = h / TILE_SIZE;

	if(right)	tile_x += width_tiles;
	
	for(j=0;j<height_tiles;j++)
	{
		if(map[ tile_x + ((tile_y+j)*SCENE_WIDTH) ] != 0)	return true;
	}
	
	return false;
}

bool cBicho::CollidesMapFloor(int *map)
{
	int tile_x,tile_y;
	int width_tiles;
	bool on_base;
	int i;

	tile_x = x / TILE_SIZE;
	tile_y = y / TILE_SIZE;

	width_tiles = w / TILE_SIZE;
	if( (x % TILE_SIZE) != 0) width_tiles++;

	on_base = false;
	i=0;
	while((i<width_tiles) && !on_base)
	{
		if( (y % TILE_SIZE) == 0 )
		{
			if(map[ (tile_x + i) + ((tile_y - 1) * SCENE_WIDTH) ] != 0)
				on_base = true;
		}
		else
		{
			if(map[ (tile_x + i) + (tile_y * SCENE_WIDTH) ] != 0)
			{
				y = (tile_y + 1) * TILE_SIZE;
				on_base = true;
			}
		}
		i++;
	}
	return on_base;
}

cRect cBicho::GetArea() const
{
	cRect rc;
	rc.left   = x;
	rc.right  = x+w;
	rc.bottom = y;
	rc.top    = y+h;
	return rc;
}
void cBicho::DrawRect(int tex_id,float xo,float yo,float xf,float yf) const
{
	int screen_x,screen_y;

	screen_x = x + SCENE_Xo;
	screen_y = y + SCENE_Yo + (BLOCK_SIZE - TILE_SIZE);

	glEnable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D,tex_id);
	glBegin(GL_QUADS);	
		glTexCoord2f(xo,yo);	glVertex2i(screen_x  ,screen_y);
		glTexCoord2f(xf,yo);	glVertex2i(screen_x+w,screen_y);
		glTexCoord2f(xf,yf);	glVertex2i(screen_x+w,screen_y+h);
		glTexCoord2f(xo,yf);	glVertex2i(screen_x  ,screen_y+h);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void cBicho::MoveLeft(int *map)
{
	int xaux;
	
	//Whats next tile?
	if( (x % TILE_SIZE) == 0)
	{
		xaux = x;
		x -= STEP_LENGTH;

		if(CollidesMapWall(map,false))
		{
			x = xaux;
			state = STATE_LOOKLEFT;
		}
	}
	//Advance, no problem
	else
	{
		x -= STEP_LENGTH;
		if(state != STATE_WALKLEFT)
		{
			state = STATE_WALKLEFT;
			seq = 0;
			delay = 0;
		}
	}
}
void cBicho::MoveRight(int *map)
{
	int xaux;

	//Whats next tile?
	if( (x % TILE_SIZE) == 0)
	{
		xaux = x;
		x += STEP_LENGTH;

		if(CollidesMapWall(map,true))
		{
			x = xaux;
			state = STATE_LOOKRIGHT;
		}
	}
	//Advance, no problem
	else
	{
		x += STEP_LENGTH;

		if(state != STATE_WALKRIGHT)
		{
			state = STATE_WALKRIGHT;
			seq = 0;
			delay = 0;
		}
	}
}
void cBicho::Stop()
{
	switch(state)
	{
		case STATE_WALKLEFT:	state = STATE_LOOKLEFT;		break;
		case STATE_WALKRIGHT:	state = STATE_LOOKRIGHT;	break;
	}
}
void cBicho::Jump(int *map)
{
	if(!jumping)
	{
		if(CollidesMapFloor(map))
		{
			jumping = true;
			jump_alfa = 0;
		}
	}
}
void cBicho::StopJumping(int *map)
{
	if(jumping && jump_alfa<60) jump_alfa = 60;
}
void cBicho::Logic(int *map, bool forward)
{
	if(forward)
	{
		if(jumping)
		{
			jump_alfa += JUMP_STEP;

			float alfa = ((float)jump_alfa) * 0.017453f;
			if(jump_alfa < 90)
			{
				y += (int)( ((float)JUMP_HEIGHT) * cos(alfa) );
			}
			else if(jump_alfa >= 90 && jump_alfa<180)
			{
				jumping = !CollidesMapFloor(map);
				if(jumping) y += (int)( ((float)JUMP_HEIGHT) * cos(alfa) );
			}
			else if(jump_alfa >= 180)
			{
				jumping = !CollidesMapFloor(map);
				if(jumping) y -= (JUMP_HEIGHT);
			}
		}
		else
		{
			jumping = !CollidesMapFloor(map);
			if(jumping) y -= (JUMP_HEIGHT);
		}
		//save this state in the states vector
		addState();
	}
	//going backwards in "time"
	else
	{
		//if we are not in the beggining
		if(states.size()>0)
		{
			//lets apply the last state of the Bicho
			cBichoState bState = states[states.size()-1];
			states.pop_back();
			setBichoState(bState);
		}
	}
}
void copyState()
{

}
void cBicho::NextFrame(int max)
{
	delay++;
	if(delay == FRAME_DELAY)
	{
		seq++;
		seq%=max;
		delay = 0;
	}
}
int cBicho::GetFrame() const
{
	return seq;
}

//i think those are realted with sprites (walking, etc)
int cBicho::GetState() const
{
	return state;
}
void cBicho::SetState(int s)
{
	state = s;
}

//this state is related with going backwards in time
void cBicho::addState()
{
	states.push_back(getBichoState());
}

cBichoState cBicho::getBichoState() const
{
	cBichoState bState;
	bState.x = x;
	bState.y = y;
	bState.w=w;
	bState.h=h;
	bState.state = state;
	bState.jumping = jumping;
	bState.jump_alfa = jump_alfa;
	bState.seq = seq;
	bState.delay = delay;
	return bState;
}

void cBicho::setBichoState(cBichoState bState)
{
	x = bState.x;
	y = bState.y;
	w = bState.w;
	h = bState.h;
	state = bState.state;
	jumping = bState.jumping;
	jump_alfa = bState.jump_alfa;
	seq = bState.seq;
	delay = bState.delay;
}