
#include "Globals.h"
#include "cGame.h"
#include "guicon.h"

//Delete console
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

#define TPF 1000/45

cGame Game;

void AppRender()
{
	Game.Render();
}
void AppKeyboard(unsigned char key, int x, int y)
{
	Game.ReadKeyboard(key,x,y,true);
}
void AppKeyboardUp(unsigned char key, int x, int y)
{
	Game.ReadKeyboard(key,x,y,false);
}
void AppSpecialKeys(int key, int x, int y)
{
	Game.ReadKeyboard(key,x,y,true);
}
void AppSpecialKeysUp(int key, int x, int y)
{
	Game.ReadKeyboard(key,x,y,false);
}
void AppMouse(int button, int state, int x, int y)
{
	Game.ReadMouse(button,state,x,y);
}
void AppMotion (int x, int y)
{
	Game.MoveMouse(x, y);
}

float tick ()
{
    return ((float) glutGet (GLUT_ELAPSED_TIME)) / 1000.0f;
}

float last_tick = 0.0f;

void AppIdle()
{
	float start = tick ();
    float dt = start - last_tick;
    last_tick = start;

	if(!Game.Loop(dt)) exit(0);

	float frame_time = tick () - start;
	//int delta_t = glutGet(GLUT_ELAPSED_TIME)- time;
	//int sleepTime = TPF - delta_t;
	int sleepTime = TPF - ((int)(frame_time * 1000.0f));
	if(sleepTime > 10) Sleep(sleepTime);
}

void main(int argc, char** argv)
{
	int res_x,res_y,pos_x,pos_y;

	// Open console in debug mode
#ifdef _DEBUG
	open_console ();
#endif

	//GLUT initialization
	glutInit(&argc, argv);

	//RGBA with double buffer
	glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE);

	//Create centered window
	res_x = glutGet(GLUT_SCREEN_WIDTH);
	res_y = glutGet(GLUT_SCREEN_HEIGHT);
	pos_x = (res_x>>1)-(GAME_WIDTH>>1);
	pos_y = (res_y>>1)-(GAME_HEIGHT>>1);
	
	glutInitWindowPosition(pos_x,pos_y);
	glutInitWindowSize(GAME_WIDTH,GAME_HEIGHT);
	glutCreateWindow("Bubble returns!");

	/*glutGameModeString("800x600:32");
	glutEnterGameMode();*/

	//Make the default cursor disappear
	//glutSetCursor(GLUT_CURSOR_NONE);

	//Register callback functions
	glutDisplayFunc(AppRender);			
	glutKeyboardFunc(AppKeyboard);		
	glutKeyboardUpFunc(AppKeyboardUp);	
	glutSpecialFunc(AppSpecialKeys);	
	glutSpecialUpFunc(AppSpecialKeysUp);
	glutMouseFunc(AppMouse);
	glutMotionFunc(AppMotion);
	glutPassiveMotionFunc(AppMotion);
	glutIdleFunc(AppIdle);

	//Game initializations
	Game.Init();

	//Application loop
	glutMainLoop();
}
