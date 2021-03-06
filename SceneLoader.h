#pragma once

class cScene;

class SceneLoader
{
	int level;
	float elapsed;
	float animationTime;
	float ty;
	int height;
	cScene* scene_prev;
	cScene* scene_cur;

public:

	SceneLoader (void);

	~SceneLoader (void);

	void restart ();

	bool nextLevel ();

	void update (float dt);

	void render (int tex_id);

	bool isLoadingDone () const;

	cScene& currentScene ();

	int getCurrentLevel ();

	void setAnimationTime (float val);

	void setViewportHeight (int val);
};
