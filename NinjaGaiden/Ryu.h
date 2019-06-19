#pragma once
#include "GameObject.h"

#define RYU_WALKING_SPEED		0.1f 
//0.1f
#define MARIO_JUMP_SPEED_Y		0.35f 
#define MARIO_GRAVITY			0.001f 
#define MARIO_DIE_DEFLECT_SPEED	 0.2f 
#define MARIO_DIE_DEFLECT_PUSH	 0.08f 

enum NINJA_ANIM
{
	NINJA_ANI_IDLE, 
	NINJA_ANI_MOVE,
	NINJA_ANI_JUMP,
	NINJA_ANI_HURT, 
	NINJA_ANI_ATTACK, 
};

#define MARIO_BIG_BBOX_WIDTH  15 
#define MARIO_BIG_BBOX_HEIGHT 27

#define MARIO_UNTOUCHABLE_TIME 3000 

enum NINJA_STATES
{
	NINJA_IDLE, 
	NINJA_JUMP, 
	NINJA_HURT, 
	NINJA_ATTACK, 
};


class CRyu : public CGameObject
{
	int ani, facing, jumping, untouchable;
	
	vector<CCollisionEvent*> GetWallCollision(vector<CCollisionEvent*>&, float&, float&, float&, float&);
	vector<CCollisionEvent*> GetEnemyCollision(vector<CCollisionEvent*>&);

public:
	int score;
	int HP = 100;
	CRyu() : CGameObject()
	{
		score = 0;
		facing = 1;
		untouchable = 0;
		state = NINJA_IDLE;
		ani = NINJA_ANI_IDLE;

		AddAnimation(Animations["ninja_idle"]);
		AddAnimation(Animations["ninja_move"]);
		AddAnimation(Animations["ninja_jump"]);
		AddAnimation(Animations["ninja_hurt"]);
		AddAnimation(Animations["ninja_attack"]);
	}

	virtual void Update(DWORD dt);
	virtual void Render();
	void SetState(int state);

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};