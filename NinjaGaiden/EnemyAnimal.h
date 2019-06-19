#pragma once
#include "Enemy.h"

class EnemyAnimal : public Enemy
{
public:
	EnemyAnimal(float x, float y, int facing);
	virtual void Update(DWORD dt);
};