#include "cGame.h"
#include "cScene.h"
#include "Globals.h"


cGame::cGame(void) : lastTick (0.0f)
{
}

cGame::~cGame(void)
{
}

bool cGame::Init()
{
	bool res=true;

	//Graphics initialization
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glDisable (GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,GAME_WIDTH,0,GAME_HEIGHT,0,1);
	glMatrixMode(GL_MODELVIEW);
	
	glAlphaFunc(GL_GREATER, 0.05f);
	glEnable(GL_ALPHA_TEST);

	//Scene initialization
	res = Data.LoadImage(IMG_BLOCKS,"blocks.png",GL_RGBA);
	if(!res) return false;
	res = sceneLoader.nextLevel();
	if(!res) return false;

	//Player initialization
	res = Data.LoadImage(IMG_PLAYER,"bub.png",GL_RGBA);
	if(!res) return false;
	Player1.SetWidthHeight(32,32);
	Player1.SetTile(4,1);
	Player1.SetState(STATE_LOOKRIGHT);

	Player2.SetWidthHeight(32,32);
	Player2.SetTile(30,1);
	Player2.SetState(STATE_LOOKLEFT);

	return res;
}

bool cGame::Loop(float dt)
{
	bool res=true;

	res = Process(dt);
	if(res) Render();

	return res;
}

void cGame::Finalize()
{
}

//Input
void cGame::ReadKeyboard(unsigned char key, int x, int y, bool press)
{
	keys[key] = press;
}

void cGame::ReadMouse(int button, int state, int x, int y)
{
}

//Process
bool cGame::Process(float dt)
{
	bool res=true;
	//Process Input
	if(keys[27])	res=false;

	// Scene loader
	sceneLoader.update(dt);
	cScene& Scene = sceneLoader.currentScene();
	
	if(keys[GLUT_KEY_UP])			Player1.Jump(Scene.GetMap());
	if(!keys[GLUT_KEY_UP])			Player1.StopJumping(Scene.GetMap());
	if(keys[GLUT_KEY_LEFT])			Player1.MoveLeft(Scene.GetMap());
	else if(keys[GLUT_KEY_RIGHT])	Player1.MoveRight(Scene.GetMap());
	else Player1.Stop();
	forward = !keys[9]; //tab key
	//hadouken
	if(forward)
	{
		if(forward && keys[GLUT_KEY_RIGHT]) Player1.addInput(GLUT_KEY_RIGHT);
		if(forward && keys[GLUT_KEY_LEFT]) Player1.addInput(GLUT_KEY_LEFT);
		if(forward && keys[GLUT_KEY_DOWN]) Player1.addInput(GLUT_KEY_DOWN); 
		if(forward && keys[32]) Player1.addInput(32);
	}
	//Game Logic
	Player1.Logic(Scene.GetMap(),forward);
	Player2.Logic(Scene.GetMap(),forward);

	// Scene loader debug
	static float last_load = 0.0f;
	last_load += dt;
	if (keys['j'] && last_load > 2.0f)
	{
		last_load = 0.0f;
		sceneLoader.nextLevel();
	}

	return res;
}

void cGame::drawBackInTime()
{
	glColor4f(0.7f,0.3f,0.0f,0.2f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_QUADS);
	glVertex2d(0,0);
	glVertex2d(GAME_WIDTH,0);
	glVertex2d(GAME_WIDTH,GAME_HEIGHT);
	glVertex2d(0,GAME_HEIGHT);
	glEnd();
	glDisable(GL_BLEND);
}

//Output
void cGame::Render()
{
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT);

	if(forward) glColor4f(1,1,1,1);
	else glColor4f(1,1,1,0.5f);

	glLoadIdentity();

	sceneLoader.render(Data.GetID(IMG_BLOCKS));
	Player1.Draw(Data.GetID(IMG_PLAYER), forward);
	Player2.Draw(Data.GetID(IMG_PLAYER), forward);

	if(!forward) drawBackInTime();

	glutSwapBuffers();
}
