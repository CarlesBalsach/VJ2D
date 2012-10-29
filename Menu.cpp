#include "Menu.h"


const float xmargin = 0.2f;
const float ymargin = 0.1f;


Menu::Menu (void) : gameStarted (false)
{
	images[LabelPlay].Load ("play.png");
	images[LabelHelp].Load ("help.png");
	images[LabelCredits].Load ("credits.png");
	images[LabelQuit].Load ("quit.png");
	images[LabelContinue].Load("continue.png");
	images[LabelNames].Load("names.png");
	images[Flowers].Load("flowers.jpg");
	images[Controls].Load("controls.png");
}


void freeTex (GLuint tex)
{
	glDeleteTextures (1, &tex);
}


Menu::~Menu (void)
{
	for (int i = 0; i < NUM_IMAGES; ++i) freeTex (images[i].GetID());
}


Menu* Menu::create ()
{
	return new Menu;
}


Menu::Action Menu::process (float xmouse, float ymouse, float width, float height)
{
	float xstart = width * xmargin;
	float ystart = height - height * ymargin;
	float xend = width - width * xmargin;
	float yend = height * ymargin;
	float step = (yend - ystart) / 4.0f;
	if (xmouse < xstart) return NoAction;
	if (xmouse > xend)   return NoAction;
	if (ymouse > ystart) return NoAction;
	if (ymouse < yend)   return NoAction;
	if (ymouse > ystart + step)   return Play;
	if (ymouse > ystart + 2*step) return Help;
	if (ymouse > ystart + 3*step) return Credits;
	return Quit;
}


void renderBackground (GLuint tex, float width, float height)
{
	glEnable (GL_TEXTURE_2D);
	glBindTexture (GL_TEXTURE_2D, tex);
	glBegin (GL_QUADS);
	glTexCoord2f (1, 0); glVertex2f (0, 0);
	glTexCoord2f (0, 0); glVertex2f (width, 0);
	glTexCoord2f (0, 1); glVertex2f (width, height);
	glTexCoord2f (1, 1); glVertex2f (0, height);
	glEnd ();
	glDisable (GL_TEXTURE_2D);
}


void Menu::renderQuad (Image img, bool highlight, float x0, float y0, float x1, float y1)
{
	GLuint tex = images[img].GetID();
	if (highlight) glColor3f (0.7f, 0.8f, 1.0f);
	else           glColor3f (0.2f, 0.5f, 0.8f);
	glBindTexture (GL_TEXTURE_2D, tex);
	glBegin (GL_QUADS);
	glTexCoord2f (0, 0);
	glVertex2f (x0, y0);
	glTexCoord2f (0, 1);
	glVertex2f (x0, y1);
	glTexCoord2f (1, 1);
	glVertex2f (x1, y1);
	glTexCoord2f (1, 0);
	glVertex2f (x1, y0);
	glEnd ();
	glColor4f (1, 1, 1, 1);
}


void Menu::render (float xmouse, float ymouse, float width, float height)
{
	float xstart = width * xmargin;
	float ystart = height - height * ymargin;
	float xend = width - width * xmargin;
	float yend = height * ymargin;
	float step = (yend - ystart) / 4.0f;

	Action action = process (xmouse, ymouse, width, height);

	renderBackground (images[Flowers].GetID(), width, height);
	glEnable (GL_TEXTURE_2D);
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if (gameStarted)
	{
		renderQuad (LabelContinue, action == Play, xstart, ystart, xend, ystart + step);
	}
	else
	{
		renderQuad (LabelPlay, action == Play, xstart, ystart, xend, ystart + step);
	}
	renderQuad (LabelHelp,    action == Help,    xstart, ystart + step,   xend, ystart + 2*step);
	renderQuad (LabelCredits, action == Credits, xstart, ystart + 2*step, xend, ystart + 3*step);
	renderQuad (LabelQuit,    action == Quit,    xstart, ystart + 3*step, xend, ystart + 4*step);
	glDisable (GL_BLEND);
	glDisable (GL_TEXTURE_2D);
}


void Menu::renderCredits (float width, float height)
{
	renderBackground (images[Flowers].GetID(), width, height);
	glEnable (GL_TEXTURE_2D);
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	renderQuad (LabelNames, false, 0, height, width, 0);
	glDisable (GL_BLEND);
	glDisable (GL_TEXTURE_2D);
}


void Menu::renderHelp (float width, float height)
{
	renderBackground (images[Flowers].GetID(), width, height);
	glEnable (GL_TEXTURE_2D);
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	renderQuad (Controls, false, 0, height, width, 0);
	glDisable (GL_BLEND);
	glDisable (GL_TEXTURE_2D);
}


void Menu::setGameStarted (bool val)
{
	gameStarted = val;
}
