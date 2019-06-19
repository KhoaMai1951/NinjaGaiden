#pragma once
#include "GameObject.h"

class ItemPickup : public CGameObject
{
	RECT bbox;

public:
	ItemPickup(float x, float y) : CGameObject()
	{
		this->x = x;
		this->y = y;
		bbox.top = -4;
		bbox.left = -4;
		bbox.right = 4;
		bbox.bottom = 4;
		animations.push_back(Animations["item_power_5"]);
		animations.push_back(Animations["item_power_10"]);
		animations.push_back(Animations["item_point_500"]);
		animations.push_back(Animations["item_point_1000"]);
		state = rand() % animations.size();
	};

	void Update(DWORD dt)
	{
		CGameObject::Update(dt);

		// Simple fall down
		vy += 0.002f*dt;

		vector<CCollisionEvent*> coEvents;

		coEvents.clear();
		coEvents = CalcPotentialCollisions(&GameObjects);

		// No collision occured, proceed normally
		if (coEvents.size() == 0)
			y += dy;
		else
		{
			float min_ty = 1.0f;
			float ny = 0.0f;

			for (int i = coEvents.size() - 1; i >= 0; --i)
			{
				CCollisionEvent* c = coEvents[i];
				if (dynamic_cast<CWall*>(c->obj))
					if (c->t < min_ty  && c->ny != 0)
					{
						min_ty = c->t;
						ny = c->ny;
					}
			}

			y += min_ty * dy + ny * 0.4f;

			if (ny != 0)
				vy = 0;
		}

		for (UINT i = 0; i < coEvents.size(); i++)
			delete coEvents[i];
	};
	void Render() { animations[state]->Render(x, y); }; 
	void GetBoundingBox(float &l, float &t, float &r, float &b)
	{
		t = y + bbox.top;
		l = x + bbox.left;
		r = x + bbox.right;
		b = y + bbox.bottom;
	};
};