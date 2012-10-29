
#include "cPlayer.h"
#include "glut.h"
#include <assert.h>
#include "cBicho.h"
#include "cScene.h"

cPlayer::cPlayer() {
	inputs = vector<int>(3,0);
	shootAnimationFrame = -1;
	dead = false;
	lives = 3;
}
cPlayer::~cPlayer(){}

void cPlayer::Logic(int *map, bool forward)
{
	if(!dead) cBicho::Logic(map,forward);
	for(unsigned int i=0; i<hadoukens.size(); i++) hadoukens[i].logic();
}

void cPlayer::MoveRight(int *map)
{
	int myState = state;
	cBicho::MoveRight(map);
	if(myState == STATE_SHOOTING_LEFT || myState == STATE_SHOOTING_RIGHT) state = myState;
}
void cPlayer::MoveLeft(int *map)
{
	int myState = state;
	cBicho::MoveLeft(map);
	if(myState == STATE_SHOOTING_LEFT || myState == STATE_SHOOTING_RIGHT) state = myState;
}
void cPlayer::Jump(int *map)
{
	int myState = state;
	cBicho::Jump(map);
	if(myState == STATE_SHOOTING_LEFT || myState == STATE_SHOOTING_RIGHT) state = myState;
}
void cPlayer::StopJumping(int *map)
{
	int myState = state;
	cBicho::StopJumping(map);
	if(myState == STATE_SHOOTING_LEFT || myState == STATE_SHOOTING_RIGHT) state = myState;
}
void cPlayer::Stop()
{
	int myState = state;
	cBicho::Stop();
	if(myState == STATE_SHOOTING_LEFT || myState == STATE_SHOOTING_RIGHT) state = myState;
}

void cPlayer::toSpawnZone(int tileX, int tileY)
{
	int destX = tileX * TILE_SIZE;
	int destY = tileY * TILE_SIZE;
	float dx = (destX - x)/30.0f;
	float dy = (destY - y)/30.0f;

	if(dx < 0 && dx > -1) dx = -1;
	else if(dx > 0 && dx < 1) dx = 1;
	if(dy < 0 && dy > -1) dy = -1;
	else if(dy > 0 && dy < 1) dy = 1;

	x += dx;
	y += dy;
	if(x == destX && y == destY)
	{
		state = STATE_LOOKRIGHT;
		dead = false;
		jumping = false;
		states.clear();
	}
}

void cPlayer::Draw(int tex_id, bool forward)
{	
	float xo,yo,xf,yf;

	//backwards int time was causing inconsistence between both variables, after spending some time i decided to patch it with this shit
	if(shootAnimationFrame != -1 && !(state == STATE_SHOOTING_LEFT || state == STATE_SHOOTING_RIGHT)) shootAnimationFrame = -1;

	switch(GetState())
	{
		//1
		case STATE_LOOKLEFT:	xo = 0.0f;	yo = 0.25f;
								break;
		//4
		case STATE_LOOKRIGHT:	xo = 0.25f;	yo = 0.25f;
								break;
		//1..3
		case STATE_WALKLEFT:	xo = 0.0f;	yo = 0.25f + (GetFrame()*0.25f);
								NextFrame(3);
								break;
		//4..6
		case STATE_WALKRIGHT:	xo = 0.25f; yo = 0.25f + (GetFrame()*0.25f);
								NextFrame(3);
								break;
		case STATE_SHOOTING_LEFT:
		{
			int hadAnim = (shootAnimationFrame/7)%2;
			xo = 0.0f + (hadAnim*0.25f); yo = 1.0f;
			shootAnimationFrame++;
			if(shootAnimationFrame==21)
			{
				state = STATE_LOOKLEFT;
				shootAnimationFrame = -1;
			}
			break;
		}
		case STATE_SHOOTING_RIGHT:
		{
			int hadAnim = (shootAnimationFrame/7)%2;
			xo = 0.5f + (hadAnim*0.25f); yo = 1.0f;
			shootAnimationFrame++;
			if(shootAnimationFrame==21)
			{
				state = STATE_LOOKRIGHT;
				shootAnimationFrame = -1;
			}
			break;
		}
		case STATE_DEAD:
			xo = 0.75f;	yo = 0.25f + (GetFrame()*0.25f);
			NextFrame(3);
			break;
	}
	xf = xo + 0.25f;
	yf = yo - 0.25f;

	if(!forward)
	{
		cBichoState mState = cBicho::getBichoState();
		for(int i=0; i<7 && i<states.size()-i && states.size()!=0; i++)
		{
			//assert(states.size()-1-i > 0);
			cBichoState pState = states[states.size()-1-i];
			cBicho::setBichoState(pState);
			glColor4f(0.88f,0.88f,0.88f,0.8f/i);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
			DrawRect(tex_id,xo,yo,xf,yf);
			glDisable(GL_BLEND);
		}
		cBicho::setBichoState(mState);
	}

	DrawRect(tex_id,xo,yo,xf,yf);

	for(unsigned int i=0; i<hadoukens.size(); i++) hadoukens[i].draw();
}

void cPlayer::addInput(int input)
{
	int s = inputs.size();
	if(inputs[s-1] != input) inputs.push_back(input);
	if(inputs[s-1] == 32 && inputs[s-2] == GLUT_KEY_LEFT && inputs[s-3] == GLUT_KEY_DOWN)
	{
		addHadouken();
		inputs = vector<int>(3,0);
	}
	else if(inputs[s-1] == 32 && inputs[s-2] == GLUT_KEY_RIGHT && inputs[s-3] == GLUT_KEY_DOWN)
	{
		addHadouken();
		inputs = vector<int>(3,0);
	}
}

void cPlayer::addHadouken()
{
	if(shootAnimationFrame == -1) {
		int hx = x+w/2;
		int hy = y+h;
		int hadoukenSpeed = 3;
		if(state == STATE_LOOKRIGHT || state == STATE_WALKRIGHT)
		{
			hx+=w;
			state = STATE_SHOOTING_RIGHT;
			shootAnimationFrame = 0;
		}
		if(state == STATE_LOOKLEFT || state == STATE_WALKLEFT)
		{
			hadoukenSpeed*=-1;
			state = STATE_SHOOTING_LEFT;
			shootAnimationFrame = 0;
		}
		hadoukens.push_back(Hadouken(hx,hy,hadoukenSpeed));
	}
}

void cPlayer::MonstersCollisions(vector<Monster1>& monsters1)
{
	vector<Monster1>::iterator mit;
	if(!dead)
	{
		for(mit = monsters1.begin(); mit != monsters1.end(); mit++)
		{
			if(mit->isAlive() && Collides(mit->GetArea()))
			{
				lives--;
				if(lives == 0)
				{
					//some code here
				}
				dead = true;
				state = STATE_DEAD;
			}
		}
	}

	for(mit = monsters1.begin(); mit != monsters1.end(); mit++)
	{
		vector<Hadouken>::iterator hit;
		for(hit = hadoukens.begin(); hit != hadoukens.end(); hit++)
		{
			if(mit->isAlive() && mit->Collides(hit->getArea()))
			{
				printf ("monster hit\n");
				mit->setAlive (false);
				hit = hadoukens.erase(hit);
			}
			if(hit == hadoukens.end()) break;
		}
		if(mit == monsters1.end()) break;
	}
}

void cPlayer::setDead (bool val)
{
	dead = val;
}

bool cPlayer::isDead()
{
	return dead;
}

void cPlayer::IA(int * map, cRect enemy)
{
	if(enemy.left < x) MoveLeft(map);
	else MoveRight(map);
	if(enemy.top > y && enemy.bottom < y + h) addHadouken();
}


void cPlayer::setLives (int val)
{
	lives = val;
}


int cPlayer::getLives () const
{
	return lives;
}