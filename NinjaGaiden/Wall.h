#pragma once
#include "GameObject.h"

class CWall : public CGameObject
{
	D3DXVECTOR4 f_bbox; 

public:
	CWall(float x, float y, float width, float height) : CGameObject()
	{
		f_bbox.x = x;
		f_bbox.y = y;
		f_bbox.z = x + width;
		f_bbox.w = y + height;
	}

	virtual void Render()
	{
	};

	virtual void GetBoundingBox(float &l, float &t, float &r, float &b)
	{
		l = f_bbox.x;
		t = f_bbox.y;
		r = f_bbox.z;
		b = f_bbox.w;
	};
};