#include "Monster2.h"
#include "cScene.h"


static const float bulletSize = 32;


Monster2::Bullet::Bullet () : speed(120.0f), alive (false) {}


Monster2::Bullet::Bullet  (float _x, float _y, float _vx, float _vy, const cRect& _rect, float _speed)
	: x(_x), y(_y), vx(_vx), vy(_vy), speed (_speed), rect(_rect), alive (true) {}


void Monster2::Bullet::update (float dt, float width, float height)
{
	if (!alive) return;
	x = x + speed * dt * vx;
	y = y + speed * dt * vy;
	if (x < 0) alive = false;
	else if (x > width) alive = false;
	if (y < 0) alive = false;
	else if (y > height) alive = false;
}


Monster2::Monster2(void)
{
}


Monster2::~Monster2(void)
{
}


void Monster2::Randomise ()
{
	// Randomise monster
	int x = 16 + rand() % (SCENE_WIDTH-20);
	int y = 2+rand() % (SCENE_HEIGHT-10);
	SetTile (x, y);

	int d = rand() % 2;
	if (d == 0)
	{
		dir = left;
		SetState(STATE_WALKLEFT);
	}
	else
	{
		dir = right;
		SetState(STATE_WALKRIGHT);
	}
	alive = true;
	jumping = false;

	// Reset bullets
	for (int i = 0; i < NUM_BULLETS; ++i)
	{
		bullets[i].alive = false;
	}
	lastShotElapsed = 0.0f;
}


bool Monster2::allBulletsUsed () const
{
	for (int i = 0; i < NUM_BULLETS; ++i)
	{
		if (!bullets[i].alive) return false;
	}
	return true;
}


int Monster2::nextAliveBullet () const
{
	for (int i = 0; i < NUM_BULLETS; ++i)
	{
		if (!bullets[i].alive) return i;
	}
	return 0;
}


void Monster2::Logic (int *map, bool forward, float dt, float width, float height, float xplayer, float yplayer)
{
	// Bicho logic
	if (!alive) return;
	cBicho::Logic (map, forward);
	jumping = false;

	// Bullet logic
	for (int i = 0; i < NUM_BULLETS; ++i)
	{
		bullets[i].update (dt, width, height);
	}

	// Randomly shoot at the player
	if (!allBulletsUsed() && lastShotElapsed >= 3.0f && (rand() % 50) == 0)
	{
		int i = nextAliveBullet();
		Bullet& b = bullets[i];
		b.alive = true;
		b.x = x;
		b.y = y;
		b.vx = xplayer - x;
		b.vy = yplayer - y;
		float norm = sqrt(b.vx * b.vx + b.vy * b.vy);
		norm = norm == 0.0f ? 1.0f : 1.0f / norm;
		b.vx *= norm;
		b.vy *= norm;
		lastShotElapsed = 0.0f;
	}
	else lastShotElapsed += dt;
}


void Monster2::Draw (GLuint texMonster, GLuint texBullet) const
{
	Monster2* m = const_cast<Monster2*> (this);

	// Draw bullets
	float xmonster = x;
	float ymonster = y;
	glEnable (GL_TEXTURE_2D);
	for (int i = 0; i < NUM_BULLETS; ++i)
	{
		const Bullet& b = bullets[i];
		if (!b.alive) continue;
		m->x = b.x;
		m->y = b.y;
		DrawRect(texBullet,0,1,1,0);
	}
	m->x = xmonster;
	m->y = ymonster;

	// Draw monster
	if (!alive) return;
	DrawRect(texMonster,0,1,1,0);
}


void Monster2::setAlive (bool val)
{
	alive = val;
}


bool Monster2::isAlive () const
{
	return alive;
}


bool Monster2::collides (const cRect& rect) const
{
	for (int i = 0; i < NUM_BULLETS; ++i)
	{
		const Bullet& b = bullets[i];
		if (!b.alive) continue;
		if (rect.right < b.x) continue;
		if (rect.left  > b.x + bulletSize) continue;
		if (rect.bottom > b.y) continue;
		if (rect.top < b.y - bulletSize) continue;
		return true;
	}
	return false;
}
