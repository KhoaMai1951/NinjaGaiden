#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include "Sprites.h"


using namespace std;



class CGameObject;



struct CCollisionEvent
{
	float t, nx, ny;
	CGameObject* obj;
	CCollisionEvent(float t, float nx, float ny, CGameObject* obj = NULL)
	{
		this->t = t; 
		this->nx = nx; 
		this->ny = ny; 
		this->obj = obj; 
	}
	static bool compare(const CCollisionEvent* a, CCollisionEvent* b) { return a->t < b->t; } // so ánh các va chạm
};



class CGameObject
{
public:
	float x;
	float y;
	float dx;	
	float dy;	
	float vx;
	float vy;

	bool is_dead; 
	int state; 
	DWORD dt; 
	
	vector<LPANIMATION> animations;

public:
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void GetPosition(float &x, float &y) { x = this->x; y = this->y; }
	void GetSpeed(float &vx, float &vy) { vx = this->vx; vy = this->vy; }

	int GetState() { return this->state; }

	bool AABB(float left, float top, float right, float bottom); 
	CCollisionEvent* SweptAABBEx(CGameObject* coO);
	vector<CCollisionEvent*> CalcPotentialCollisions(vector<CGameObject*> *coObjects);

	
	virtual void FilterCollision(vector<CCollisionEvent*> &coEvents, vector<CCollisionEvent*> &coEventsResult, float &min_tx, float &min_ty, float &nx, float &ny);
	//min t thời gian va chạm nhỏ nhất
	virtual CGameObject* AddAnimation(LPANIMATION);

	CGameObject();
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) {};
	virtual void Update(DWORD dt);
	virtual void Render() = 0;
	virtual void SetState(int state) { this->state = state; }
	virtual ~CGameObject() {};

	static void DrawText(string text, float x, float y, int s = -1)
	{
		CSprites* sprites = CSprites::GetInstance();
		if (s == -1)
			s = text.size();
		for (int i = 0; i < s; i++)
		{
			int index = -1;
			for (int ii = 10; ii <= 35; ii++)
				if (text[i] == 55 + ii || text[i] == 87 + ii)// Chữ hoa và chữ thường
				{
					index = ii;
					break;
				}
			for (int ii = 0; ii <= 9; ii++) // 0 -> 9
				if (text[i] == 48 + ii)
				{
					index = ii;
					break;
				}
			if (index >= 0)
				sprites->Get(TEXT_INDEX + index)->Draw(x + i * 8, y, 255, 0.5f, 0.5f);
		}
	};
	static float xy2Dir(float x, float y)
	{
		return atan2(y, x) * 180.0f / 3.141592;
	};


	static void Dir2xy(float length, float direction, float &x, float &y)
	{
		x = (float)cos((direction / 180.0f)*3.141592)*length;
		y = (float)sin((direction / 180.0f)*3.141592)*length;
	};
};

extern vector<CGameObject*> GameObjects;
