#pragma once

#include "cbicho.h"

class Monster2 : public cBicho
{
	enum Direction { left, right };
	Direction dir;
	bool alive;
	float lastShotElapsed;

	class Bullet
	{
	public:
		
		float x, y;
		float vx, vy;
		float speed;
		cRect rect;
		bool alive;

		Bullet ();

		Bullet (float x, float y, float vx, float vy, const cRect& rect, float speed);

		void update (float dt, float width, float height);
	};

	static const int NUM_BULLETS = 4;

	Bullet bullets[NUM_BULLETS];

	bool allBulletsUsed () const;

	int nextAliveBullet () const;

public:

	Monster2(void);

	~Monster2(void);

	void Randomise ();

	void Logic(int *map, bool forward, float dt, float width, float height, float xplayer, float yplayer);

	void Draw (GLuint texMonster, GLuint texBullet) const;

	void setAlive (bool val);

	bool isAlive () const;

	bool collides (const cRect& rect) const;
};

