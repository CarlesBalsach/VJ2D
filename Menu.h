#pragma once

#include "cTexture.h"

class Menu
{
public:

	enum Action
	{ Play
	, Help
	, Credits
	, Quit
	, NoAction
	};

	static Menu* create ();

	~Menu (void);

	Action process (float xmouse, float ymouse, float width, float height);

	void render (float xmouse, float ymouse, float width, float height);

	void renderCredits (float width, float height);

	void setGameStarted (bool val);

private:

	enum Label
	{ LabelPlay
	, LabelHelp
	, LabelCredits
	, LabelQuit
	, LabelContinue
	, LabelCarles
	, LabelMarc
	, NUM_LABELS
	};

	cTexture labels[NUM_LABELS];
	bool gameStarted;

	Menu (void);

	void renderQuad (int action, bool selected, float x0, float y0, float x1, float y1);
};
