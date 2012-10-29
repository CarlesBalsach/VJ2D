#include "SceneLoader.h"
#include "cScene.h"


SceneLoader::SceneLoader (void)
	: level (0), elapsed (0.0f), animationTime (1.5f), ty (0.0f), height(480.0f), scene_prev (0), scene_cur (0)
{
}


SceneLoader::~SceneLoader (void)
{
	if (scene_prev && scene_prev != scene_cur) delete scene_prev;
	if (scene_cur)  delete scene_cur;
}


void SceneLoader::restart ()
{
	if (scene_prev && scene_prev != scene_cur) delete scene_prev;
	if (scene_cur) delete scene_cur;
	level = 0;
	scene_prev = 0;
	scene_cur = 0;
}


bool SceneLoader::nextLevel ()
{
	level++;
	scene_prev = scene_cur;
	scene_cur = new cScene;
	if (!scene_prev) scene_prev = scene_cur;
	bool res = scene_cur->LoadLevel (level);
	if (!res)
	{
		delete scene_cur;
		scene_cur = scene_prev;
	}
	return res;
}


void SceneLoader::update (float dt)
{
	if (scene_prev == scene_cur) return;

	elapsed += dt;
	ty = elapsed / animationTime * height;

	if (elapsed >= animationTime)
	{
		delete scene_prev;
		scene_prev = scene_cur;
		elapsed = 0.0f;
	}
}


void draw_scene (int tex_id, cScene* scene, float ty, float a)
{
	glColor4f (1.0f, 1.0f, 1.0f, a);
	glPushMatrix ();
	glTranslatef (0.0f, ty, 0.0f);
	scene->Draw (tex_id);
	glPopMatrix ();
}


void SceneLoader::render (int tex_id)
{
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	float p = elapsed == 0.0f ? 1.0f : elapsed / animationTime;
	if (scene_prev && scene_prev != scene_cur) draw_scene (tex_id, scene_prev, ty, 1.0f - p);
	if (scene_cur) draw_scene (tex_id, scene_cur, 0.0f, p);
	glDisable (GL_BLEND);
	glColor3f (1.0f, 1.0f, 1.0f);
}


bool SceneLoader::isLoadingDone () const
{
	return scene_prev == scene_cur;
}


cScene& SceneLoader::currentScene ()
{
	return *scene_cur;
}


int SceneLoader::getCurrentLevel ()
{
	return level;
}


void SceneLoader::setAnimationTime (float val)
{
	animationTime = val;
}


void SceneLoader::setViewportHeight (int val)
{
	height = static_cast<float> (val);
}
