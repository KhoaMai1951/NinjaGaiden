#pragma once
#include "Enemy.h"

enum ENEMY_BOSS_STATE
{
	ENEMY_BOSS_STATE_IDLE, //nhay
	ENEMY_BOSS_STATE_SHOOT // bắn
};

class EnemyBoss : public Enemy
{
	float Timer;

public:
	int HP = 10;
	EnemyBoss(float x, float y, int facing);
	virtual void Update(DWORD dt);
};

