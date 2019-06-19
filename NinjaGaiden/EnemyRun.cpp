#include "EnemyRun.h""


EnemyRun::EnemyRun(float x, float y, int facing) : Enemy(x, y, facing)
{
	AddAnimation(Animations["enemy_run"]);
};

void EnemyRun::Update(DWORD dt)
{
	if (!IsInCamera())
		return;
	
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
		{
			vx = 0;
			facing = -facing;
		}
		if (ny != 0)
			vy = 0;
	}

	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];

	vx = facing * 0.08f;
};

void EnemyRun::Render()
{
	animations[0]->Render(x, y, 255, (float)facing);
};