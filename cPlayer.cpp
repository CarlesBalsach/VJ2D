
#include "cPlayer.h"
#include "glut.h"
#include <assert.h>

cPlayer::cPlayer() {
	inputs = vector<int>(3,0);
}
cPlayer::~cPlayer(){}

void cPlayer::Logic(int *map, bool forward)
{
	cBicho::Logic(map,forward);
	for(unsigned int i=0; i<hadoukens.size(); i++) hadoukens[i].logic();
}

void cPlayer::Draw(int tex_id, bool forward)
{	
	float xo,yo,xf,yf;

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
	hadoukens.push_back(Hadouken(x+2*w,y+h,3));
}