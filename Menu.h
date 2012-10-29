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

	void renderHelp (float width, float height);

	void setGameStarted (bool val);

private:

	enum Image
	{ LabelPlay
	, LabelHelp
	, LabelCredits
	, LabelQuit
	, LabelContinue
	, LabelNames
	, Flowers
	, Controls
	, NUM_IMAGES
	};

	cTexture images[NUM_IMAGES];
	bool gameStarted;

	Menu (void);

	void renderQuad (Image img, bool selected, float x0, float y0, float x1, float y1);
};
