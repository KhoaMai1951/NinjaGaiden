#pragma once
#include "GameObject.h"

class Particle : public CGameObject
{
	float life;
public:
	Particle(float x, float y, CAnimation* animation, float life = 1000) : CGameObject()
	{
		this->x = x;
		this->y = y;
		this->life = life;
		AddAnimation(animation);
	};

	void Update(DWORD dt)
	{
		if (life < 0)
			is_dead = true;
		else
			life -= dt;
	};

	void Render() { animations[0]->Render(x, y); };
};