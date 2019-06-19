#pragma once
#include "Enemy.h"

class EnemyRun : public Enemy

{
public:
	EnemyRun(float x, float y, int facing);
	virtual void Update(DWORD dt);
	virtual void Render();
};

