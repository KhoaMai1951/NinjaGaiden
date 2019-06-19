#include "Enemy_Bullet.h"

#include "Game.h"
#include "Camera.h"

Enemy_Bullet::Enemy_Bullet(float x, float y, float vx, float vy, CAnimation* anim) : CGameObject()
{
	this->x = x;
	this->y = y;
	this->vx = vx;
	this->vy = vy;
	AddAnimation(anim);
	bbox.top = -8;
	bbox.left = -4;
	bbox.right = 4;
	bbox.bottom = 8;
};

void Enemy_Bullet::Update(DWORD dt)
{
	if (IsInCamera())
	{
		CGameObject::Update(dt);
		x += dx;
		y += dy;
	}
	else
		is_dead = true;
};

void Enemy_Bullet::Render()
{
	animations[0]->Render(x, y, 255, vx < 0 ? -1 : 1);
};

void Enemy_Bullet::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	t = y + bbox.top;
	l = x + bbox.left;
	r = x + bbox.right;
	b = y + bbox.bottom;
};

bool Enemy_Bullet::IsInCamera()
{
	Camera* camera = Camera::GetInstance();
	float top = camera->top;
	float left = camera->left;
	float right = camera->right;
	float bottom = camera->bottom;
	return AABB(left, top, right, bottom);
};