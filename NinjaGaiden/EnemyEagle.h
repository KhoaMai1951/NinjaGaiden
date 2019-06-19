#pragma once
#include "Enemy.h"
class EnemyEagle : public Enemy
{
public:
	EnemyEagle(float x, float y, int facing);
	virtual void Update(DWORD dt);
	//virtual void Render();
};

