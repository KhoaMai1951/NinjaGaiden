#pragma once
#include "GameObject.h"
#include "ItemPickup.h"

class ItemCarrier : public CGameObject
{
	D3DXVECTOR4 f_bbox;

public:
	ItemCarrier(float x, float y, CAnimation* anim) : CGameObject()
	{
		this->x = x;
		this->y = y;
		f_bbox.x = x - 8;
		f_bbox.y = y - 8;
		f_bbox.z = x + 8;
		f_bbox.w = y + 8;
		animations.push_back(anim);
	};

	virtual void Render() { animations[0]->Render(x, y); };
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b)
	{
		l = f_bbox.x;
		t = f_bbox.y;
		r = f_bbox.z;
		b = f_bbox.w;
	};
	~ItemCarrier()  
	{
		GameObjects.push_back(new ItemPickup(x, y));
	};
};