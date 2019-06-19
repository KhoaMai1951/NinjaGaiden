#pragma once
#include "Enemy.h"

class EnemyThrow : public Enemy
{
	float Timer;

public:
	EnemyThrow(float x, float y, int facing);
	virtual void Update(DWORD dt);
};