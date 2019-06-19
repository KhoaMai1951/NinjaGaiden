#include "Enemy_Projectile.h"

Enemy_Projectile::Enemy_Projectile(float x, float y, float vx, float vy, CAnimation* anim) : Enemy_Bullet(x, y, vx, vy, anim)
{
	bbox.top = -8;
	bbox.left = -4;
	bbox.right = 4;
	bbox.bottom = 8;
};

void Enemy_Projectile::Update(DWORD dt)
{
	if (IsInCamera())
	{
		CGameObject::Update(dt);
		vy += ENEMY_PROJECTILE_GRAVITY * dt;
		x += dx;
		y += dy;
	}
	else
		is_dead = true;
};