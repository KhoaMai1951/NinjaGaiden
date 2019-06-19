#pragma once
#include "GameObject.h"
#include "Particle.h"
#include "WallAI.h"
#include "Wall.h"

class Enemy : public CGameObject
{
protected:
	RECT bbox;
	int facing; 

public:
	Enemy(float x, float y, int facing);
	bool IsInCamera();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void SetState(int state); 
	
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom); 

	virtual ~Enemy();
};