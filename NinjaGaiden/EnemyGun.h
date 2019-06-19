#pragma once
#include "Enemy.h"

enum ENEMY_GUN_STATE
{
	ENEMY_GUN_STATE_IDLE,
	ENEMY_GUN_STATE_SHOOT 
};

class EnemyGun : public Enemy
{
	float Timer;

public:
	EnemyGun(float x, float y, int facing);
	virtual void Update(DWORD dt);
	virtual void Render();
};