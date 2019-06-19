#include "Enemy_Bullet.h"

#define ENEMY_PROJECTILE_GRAVITY 0.001f 

class Enemy_Projectile : public Enemy_Bullet
{
public:
	Enemy_Projectile(float, float, float, float, CAnimation*);
	void Update(DWORD dt);
};