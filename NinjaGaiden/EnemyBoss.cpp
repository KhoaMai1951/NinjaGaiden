#include "EnemyBoss.h"
#include "Enemy_Bullet.h"


EnemyBoss::EnemyBoss(float x, float y, int facing) : Enemy(x, y, facing)
{
	AddAnimation(Animations["enemy_boss"]);
	
}
//0.012
void EnemyBoss::Update(DWORD dt)
{
	CGameObject::Update(dt);
	// Simple fall down
	vx = facing*0.18f;
	vy += 0.00075f*dt;

	vector<CCollisionEvent*> coEvents;
	coEvents.clear();
	coEvents = CalcPotentialCollisions(&GameObjects);

	if (state != -1 && !HP)
	{
		state = -1;
		Timer = 2000;
	}

	float ny = 0.0f;
	// No collision occured, proceed normally
	if (state == -1)
	{

	}
	else if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx = 1.0f;
		float min_ty = 1.0f;
		float nx = 0.0f;

		for (int i = coEvents.size() - 1; i >= 0; --i)
		{
			CCollisionEvent* c = coEvents[i];
			if (dynamic_cast<CWall*>(c->obj)) //|| dynamic_cast<WallAI*>(c->obj)
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
			
		}
		if (ny != 0)
		{
			if (ny < 0)
			{
				
				facing = -facing;
				
				vy = -0.4f; 
			}
				
			else
				vy = 0;
		}
	}
	
	switch (state)
	{
	case ENEMY_BOSS_STATE_IDLE:
		if (IsInCamera())
		{
			if (Timer < 0 && ny != 0)
			{
				Timer = 200;
				state = ENEMY_BOSS_STATE_SHOOT;
				GameObjects.push_back(new Enemy_Bullet(x+10*facing, y, facing*0.1f, 0, Animations["enemy_3_bullet"]));
				GameObjects.push_back(new Enemy_Bullet(x + 20 * facing, y+10, facing*0.1f, 0, Animations["enemy_3_bullet"]));
				GameObjects.push_back(new Enemy_Bullet(x, y-10, facing*0.1f, 0, Animations["enemy_3_bullet"]));

			}
			else
				Timer -= dt;
		};
		break;

	case ENEMY_BOSS_STATE_SHOOT:
		if (Timer < 0)
		{
			Timer = 3000; //tuan suat ban
			state = ENEMY_BOSS_STATE_IDLE;
		}
		else
			Timer -= dt;
		break;

	
	}

	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];
};

