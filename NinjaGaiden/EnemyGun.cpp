#include "EnemyGun.h"

#include "Enemy_Bullet.h"

EnemyGun::EnemyGun(float x, float y, int facing) : Enemy(x, y, facing)
{
	Timer = 3000;
	state = ENEMY_GUN_STATE_IDLE;
	AddAnimation(Animations["enemy_3a"]);
	AddAnimation(Animations["enemy_3b"]);
};

void EnemyGun::Update(DWORD dt)
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
			if (dynamic_cast<CWall*>(c->obj))
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



	switch (state)
	{
	case ENEMY_GUN_STATE_IDLE:
		if (IsInCamera())
		{
			if (Timer < 0)
			{
				Timer = 200;
				state = ENEMY_GUN_STATE_SHOOT;
				GameObjects.push_back(new Enemy_Bullet(x, y, facing*0.1f, 0, Animations["enemy_3_bullet"]));

			}
			else
				Timer -= dt;
		};
		break;

	case ENEMY_GUN_STATE_SHOOT:
		if (Timer < 0)
		{
			Timer = 2000;
			state = ENEMY_GUN_STATE_IDLE;
		}
		else
			Timer -= dt;
		break;
	}

	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];
};

void EnemyGun::Render()
{
	animations[state]->Render(x, y, 255, (float)facing);
};