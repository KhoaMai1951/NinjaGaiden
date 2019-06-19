#pragma once
#include "GameObject.h"

class Tile : public CGameObject
{
	CSprite* sprite; 

public:
	Tile(float x, float y, CSprite* sprite = nullptr) : CGameObject()
	{
		this->x = x;
		this->y = y;
		this->sprite = sprite;
	}

	virtual void Render()
	{
		if (sprite != nullptr)
		{
			sprite->Draw(floor(x), y);
		}
	};

	virtual void GetBoundingBox(float &l, float &t, float &r, float &b)
	{
	};
};