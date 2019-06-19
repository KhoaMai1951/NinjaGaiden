#include "EnemyAnimal.h"

EnemyAnimal::EnemyAnimal(float x, float y, int facing) : Enemy(x, y, facing)
{
	AddAnimation(Animations["enemy_4"]);
	bbox.bottom = 24;
	state = 0;
};

void EnemyAnimal::Update(DWORD dt)
{
	if (IsInCamera())
		state = 1;
	if (!state)
		return;
	CGameObject::Update(dt);

	// Simple fall down
	vy += 0.001f*dt;

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
			if (dynamic_cast<CWall*>(c->obj) ) //|| dynamic_cast<WallAI*>(c->obj)
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
		{
			vx = 0;
			facing = -facing;
		}
		if (ny != 0)
		{
			if (ny < 0)
				vy = -0.2f; // suc nhay
			else
				vy = 0;
		}

	}
	vx = facing * 0.06f;

	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];
};