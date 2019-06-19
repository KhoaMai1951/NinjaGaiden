#include "EnemyThrow.h"

#include "Enemy_Projectile.h"
#include "Ryu.h"

EnemyThrow::EnemyThrow(float x, float y, int facing) : Enemy(x, y, facing)
{
	vx = facing * 0.03f;
	Timer = 1000;
	AddAnimation(Animations["enemy_2"]);
};

void EnemyThrow::Update(DWORD dt)
{
	CGameObject::Update(dt);

	// Simple fall down
	vy += 0.002f*dt;

	vector<CCollisionEvent*> coEvents;
	coEvents.clear();
	coEvents = CalcPotentialCollisions(&GameObjects);

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx = 1.0f;
		float min_ty = 1.0f;
		float nx = 0.0f;
		float ny = 0.0f;

		for (int i = coEvents.size() - 1; i >= 0; --i)
		{
			CCollisionEvent* c = coEvents[i];
			if (dynamic_cast<CWall*>(c->obj) || dynamic_cast<WallAI*>(c->obj))
			{
				if (c->t < min_tx && c->nx != 0)
				{
					min_tx = c->t;
					nx = c->nx;
				}
				if (c->t < min_ty  && c->ny != 0)
				{
					min_ty = c->t;
					ny = c->ny;
				}
			}
		}

		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0 && ny == 0)
			vx = -vx;
		if (ny != 0)
			vy = 0;
	}

	if (IsInCamera())
	{
		if (Timer < 0)
		{
			Timer = 1250; 
			GameObjects.push_back(new Enemy_Projectile(x, y, facing*0.1f, -0.3f, Animations["enemy_2_projectile"]));
		}
		else
			Timer -= dt;

	
		for (CGameObject* obj : GameObjects)
			if (dynamic_cast<CRyu*>(obj))
			{
				facing = obj->x - x > 0 ? 1 : -1;
				break;
			}
	};

	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];
};