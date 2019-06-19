#include "Ryu.h"
#include "Game.h"

#include "Wall.h"
#include "Enemy.h"
#include "EnemyBoss.h"
#include "Enemy_Bullet.h"
#include "ItemPickup.h"
#include "ItemCarrier.h"
#include "SpatialGrid.h"

void CRyu::Update(DWORD dt)
{
	
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	vy += MARIO_GRAVITY * dt;

	vector<CCollisionEvent*> coEvents;
	vector<CCollisionEvent*> coEventsResult;

	float l, t, r, b;
	GetBoundingBox(l, t, r, b);


	set<CGameObject*> set_gameobject = SpatialGrid::GetInstance()->Get(l, t, r, b);

	vector<CGameObject*> vector_gameobject(set_gameobject.begin(), set_gameobject.end());
	coEvents.clear();
	coEvents = CalcPotentialCollisions(&vector_gameobject);


	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		//FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);
		coEventsResult = GetWallCollision(coEvents, min_tx, min_ty, nx, ny);


		// block 
		x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.4f;        

		if (nx != 0) vx = 0;
		if (ny != 0)
		{
			vy = 0;
			if (ny < 0)
				jumping = false;
		}
	}


	
	for (CCollisionEvent* collision_event : coEvents)
		if (dynamic_cast<ItemPickup*>(collision_event->obj))
			if (collision_event->ny != 0 || collision_event->nx != 0)
			{
				collision_event->obj->is_dead = true;
				score += 250;
			}

	GetEnemyCollision(coEvents);


	switch (state)
	{
	case NINJA_IDLE:
	{
		int direction = CGame::GetInstance()->IsKeyDown(DIK_RIGHT) - CGame::GetInstance()->IsKeyDown(DIK_LEFT);
		if (direction > 0)
		{
			vx = RYU_WALKING_SPEED;
			ani = NINJA_ANI_MOVE;
			facing = 1;
		}
		else if (direction < 0)
		{
			vx = -RYU_WALKING_SPEED;
			ani = NINJA_ANI_MOVE;
			facing = -1;
		}
		else
		{
			vx = 0;
			ani = NINJA_ANI_IDLE;
		}
		if (CGame::GetInstance()->IsKeyPress(DIK_Z))
			SetState(NINJA_JUMP);
		else if (CGame::GetInstance()->IsKeyPress(DIK_X))
		{
			SetState(NINJA_ATTACK);
		}
	}
	break;

	case NINJA_JUMP:
		ani = NINJA_ANI_JUMP;
		if (!jumping)
			state = NINJA_IDLE;
		{
			int direction = CGame::GetInstance()->IsKeyDown(DIK_RIGHT) - CGame::GetInstance()->IsKeyDown(DIK_LEFT);
			if (direction > 0)
				vx = RYU_WALKING_SPEED;
			else if (direction < 0)
				vx = -RYU_WALKING_SPEED;
		}
		if (CGame::GetInstance()->IsKeyPress(DIK_X))
		{
			SetState(NINJA_ATTACK);
		}
		break;

	case NINJA_ATTACK:
		
		ani = NINJA_ANI_ATTACK;
		if (animations[ani]->finished)
			state = NINJA_IDLE;
		if (!jumping)
			vx = 0;
		break;

	case NINJA_HURT:
		if (!jumping)
			state = NINJA_IDLE;
		ani = NINJA_ANI_HURT;
		break;
	}


	if (CGame::GetInstance()->IsKeyPress(DIK_SPACE))
		SetState(NINJA_JUMP);

	untouchable -= dt;
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];
}

void CRyu::Render()
{
	int alpha = 255;
	if (untouchable > 0)
	{
		alpha = rand() % 128 + 64;
	}
	animations[ani]->Render(x + MARIO_BIG_BBOX_WIDTH / 2, floor(y), alpha, (float)facing);
}

void CRyu::SetState(int state)
{
	//CGameObject::SetState(state);

	switch (state)
	{
	case NINJA_JUMP:
		this->state = NINJA_JUMP;
		vy = -MARIO_JUMP_SPEED_Y;
		jumping = true;
		ani = NINJA_ANI_JUMP;
		break;

	case NINJA_ATTACK:
		if (this->state == NINJA_IDLE || this->state == NINJA_JUMP)
		{
			this->state = NINJA_ATTACK;
			ani = NINJA_ANI_ATTACK;
			animations[ani]->Restart();
			float top = y;
			float left = x + MARIO_BIG_BBOX_WIDTH / 2;
			float right = left + 40 * facing;
			float bottom = y + MARIO_BIG_BBOX_HEIGHT;
			for (int i = GameObjects.size() - 1; i >= 0; --i)
			{
				CGameObject* obj = GameObjects[i];

				if (obj->AABB(left, top, right, bottom))
				{
					if (dynamic_cast<EnemyBoss*>(obj))
					{
						dynamic_cast<EnemyBoss*>(obj)->HP--;
					}
					else if (dynamic_cast<Enemy*>(obj))
					{
						obj->SetState(-1);
						score += 200;
					}
					else if (dynamic_cast<ItemCarrier*>(obj))
						obj->is_dead = true;
				}
			}
		}
		break;

	case NINJA_HURT:
		HP--;
		this->state = NINJA_HURT;
		vx = -facing * MARIO_DIE_DEFLECT_PUSH;
		vy = -MARIO_DIE_DEFLECT_SPEED;
		jumping = true; 
		untouchable = MARIO_UNTOUCHABLE_TIME;
		break;
	}
}

void CRyu::GetBoundingBox(float &left, float &top, float &right, float &bottom)

{
	left = x;
	top = y;
	right = x + MARIO_BIG_BBOX_WIDTH;
	

	bottom = y + MARIO_BIG_BBOX_HEIGHT;
	
}


vector<CCollisionEvent*> CRyu::GetWallCollision(vector<CCollisionEvent*> &coEvents, float &min_tx, float &min_ty, float &nx, float &ny)
{
	int min_ix = -1;
	int min_iy = -1;
	min_tx = 1.0f;
	min_ty = 1.0f;
	nx = 0.0f;
	ny = 0.0f;

	vector<CCollisionEvent*> result;
	for (int i = coEvents.size() - 1; i >= 0; --i)
	{
		CCollisionEvent* c = coEvents[i];
		if (dynamic_cast<CWall*>(c->obj))
		{
			if (c->t < min_tx && c->nx != 0)
			{
				min_tx = c->t;
				nx = c->nx;
				min_ix = i;
			}
			if (c->t < min_ty  && c->ny != 0)
			{
				min_ty = c->t;
				ny = c->ny;
				min_iy = i;
			}
		}
	}
	if (min_ix >= 0)
		result.push_back(coEvents[min_ix]);
	if (min_iy >= 0)
		result.push_back(coEvents[min_iy]);
	return result;
};


vector<CCollisionEvent*> CRyu::GetEnemyCollision(vector<CCollisionEvent*> &coEvents)
{
	vector<CCollisionEvent*> result;
	for (int i = coEvents.size() - 1; i >= 0; --i)
		if (dynamic_cast<Enemy*>(coEvents[i]->obj) || dynamic_cast<Enemy_Bullet*>(coEvents[i]->obj))
			if ((coEvents[i]->ny != 0 || coEvents[i]->nx != 0) && untouchable < 0)
			{
				SetState(NINJA_HURT);
				result.push_back(coEvents[i]);
			}
	return result;
};