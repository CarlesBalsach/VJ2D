#include "cGame.h"
#include "cScene.h"
#include "Globals.h"
#include <cstring> // memset


cGame::cGame(void) : forward (false), lastTick (0.0f), status (Menu)
{
	memset (buttons, 0, sizeof(buttons));
	memset (keys, 0, sizeof(keys));
}

cGame::~cGame(void)
{
}

bool cGame::Init()
{
	bool res=true;

	//Graphics initialization
	setStatus (Menu);
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

	//Monster1 initialization
	monsters1.resize(NUM_MONSTERS);
	for (int i = 0; i < NUM_MONSTERS; ++i)
	{
		monsters1[i].SetWidthHeight(32, 32);
		monsters1[i].Randomise();
	}

	//Menu initialization.
	menu = Menu::create ();

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
	delete menu;
}

//Input
void cGame::ReadKeyboard(unsigned char key, int x, int y, bool press)
{
	keys[key] = press;
}

void cGame::ReadMouse(int button, int state, int x, int y)
{
	buttons[button] = state == 0;
}

void cGame::MoveMouse(int x, int y)
{
	xmouse = x;
	ymouse = y;
}


void cGame::setStatus (Status status)
{
	this->status = status;
	if (status == Playing)
	{
		glClearColor(0.2f,0.4f,0.7f,0.0f);
	}
	else
	{
		glClearColor(0.75f,0.75f,0.75f,0.0f);
	}

	if (status == Playing) printf ("Playing\n");
	else if (status == Credits) printf ("Credits\n");
	else if (status == Help) printf ("Help\n");
	else if (status == Menu) printf("Menu\n");
}


void cGame::translateMouse (float& x, float& y)
{
	GLint vp[4];
	glGetIntegerv (GL_VIEWPORT, vp);
	x = xmouse / (float) vp[2] * GAME_WIDTH;
	y = GAME_HEIGHT - (ymouse / (float) vp[3] * GAME_HEIGHT);
}

//Process
bool cGame::Process(float dt)
{
	bool res = true;

	//Process Input
	static float escDelay = 1.0f;
	escDelay += dt;
	if(keys[27] && escDelay >= 1.0f)
	{
		escDelay = 0.0f;
		if (status != Menu)
		{
			setStatus(Menu);
		}
		else return false;
	}

	// Game menu
	if (status == Menu && buttons[GLUT_LEFT_BUTTON])
	{
		float x, y;
		translateMouse (x, y);
		Menu::Action action = menu->process (x, y, GAME_WIDTH, GAME_HEIGHT);
		
		if (action == Menu::Play)
		{
			menu->setGameStarted (true);
			setStatus(Playing);
			return true;
		}
		else if (action != Menu::Quit)
		{
			if (action == Menu::Help) setStatus(Help);
			else if (action == Menu::Credits) setStatus(Credits);
			return true;
		}
		else return false;
	}
	else if (status != Playing) return true;

	// Scene loader
	sceneLoader.update(dt);
	cScene& Scene = sceneLoader.currentScene();
	
	//Player2.IA(Scene.GetMap(),Player1.GetArea());
	
	if(!Player1.isDead())
	{
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
	}
	else
	{
		if (sceneLoader.isLoadingDone()) Player1.setDead (false);
		else Player1.toSpawnZone(4,1);
	}

	//Game Logic
	Player1.Logic(Scene.GetMap(),forward);
	Player2.Logic(Scene.GetMap(),forward);
	for (int i = 0; i < monsters1.size(); ++i)
	{
		monsters1[i].Logic(Scene.GetMap(), forward);
	}

	//collisions
	Player1.MonstersCollisions(monsters1);

	// Move on to the next level when all monsters have been killed.
	bool all_dead = true;
	for (int i = 0; i < monsters1.size(); ++i)
	{
		if (monsters1[i].isAlive())
		{
			all_dead = false;
			break;
		}
	}
	if (all_dead) nextLevel();

	// Scene loader debug
	static float last_load = 0.0f;
	last_load += dt;
	if (keys['j'] && last_load > 2.0f)
	{
		last_load = 0.0f;
		nextLevel ();
	}

	return res;
}

void cGame::nextLevel ()
{
	// Generate random monsters.
	for (int i = 0; i < monsters1.size(); ++i)
	{
		monsters1[i].Randomise();
	}
	Player1.setDead (true);
	sceneLoader.nextLevel();
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
	glClear(GL_COLOR_BUFFER_BIT);

	if (status == Menu)
	{
		float x, y;
		translateMouse (x, y);
		menu->render (x, y, GAME_WIDTH, GAME_HEIGHT);
	}
	else if (status == Credits)
	{
		menu->renderCredits (GAME_WIDTH, GAME_HEIGHT);
	}
	else if (status == Playing)
	{
		glColor3f(1,1,1);
		glLoadIdentity();

		sceneLoader.render(Data.GetID(IMG_BLOCKS));
		Player1.Draw(Data.GetID(IMG_PLAYER), forward);
		Player2.Draw(Data.GetID(IMG_PLAYER), forward);

		for (int i = 0; i < monsters1.size(); ++i)
		{
			monsters1[i].Draw(Data.GetID(IMG_PLAYER));
		}

		if(!forward)
		{
			drawBackInTime();
			glColor4f (0.5f, 0.5f, 0.5f, 0.01f);
			glEnable (GL_BLEND);
			glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glBegin (GL_QUADS);
			glVertex2f (0, 0);
			glVertex2f (GAME_WIDTH, 0);
			glVertex2f (GAME_WIDTH, GAME_HEIGHT);
			glVertex2f (0, GAME_HEIGHT);
			glEnd ();
			glDisable (GL_BLEND);
		}
	}

	glutSwapBuffers();
}
