#include <d3dx9.h>
#include <algorithm>

#include "Textures.h"
#include "Game.h"
#include "GameObject.h"

vector<CGameObject*> GameObjects;

CGameObject::CGameObject()
{
	x = y = 0;
	vx = vy = 0;
	is_dead = false;
}

void CGameObject::Update(DWORD dt)
{
	this->dt = dt;
	dx = vx * dt;
	dy = vy * dt;
}

/*
	Extension of original SweptAABB to deal with two moving objects
*/
CCollisionEvent* CGameObject::SweptAABBEx(CGameObject* coO)
{
	float sl, st, sr, sb, svx, svy;	// static object bbox
	float ml, mt, mr, mb;				// moving object bbox
	float t, nx, ny;

	coO->GetBoundingBox(sl, st, sr, sb);
	coO->GetSpeed(svx, svy);

	// deal with moving object: m speed = original m speed - collide object speed
	float sdx = svx * dt;
	float sdy = svy * dt;

	float dx = this->dx - sdx;
	float dy = this->dy - sdy;

	GetBoundingBox(ml, mt, mr, mb);
	CGame::SweptAABB(ml, mt, mr, mb, dx, dy, sl, st, sr, sb, t, nx, ny);

	CCollisionEvent* e = new CCollisionEvent(t, nx, ny, coO);
	return e;
}

/*
	Calculate potential collisions with the list of colliable objects

	coObjects: the list of colliable objects
	coEvents: list of potential collisions
*/


vector<CCollisionEvent*> CGameObject::CalcPotentialCollisions(vector<CGameObject*> *coObjects)
{
	vector<CCollisionEvent*> coEvents;
	coEvents.clear();
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		CCollisionEvent* e = SweptAABBEx(coObjects->at(i));

		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}
	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare); 

	return coEvents;
}

void CGameObject::FilterCollision(vector<CCollisionEvent*> &coEvents, vector<CCollisionEvent*> &coEventsResult, float &min_tx, float &min_ty, float &nx, float &ny)
{
	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	nx = 0.0f;
	ny = 0.0f;

	coEventsResult.clear();

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		CCollisionEvent* c = coEvents[i];

		if (c->t < min_tx && c->nx != 0) {
			min_tx = c->t; nx = c->nx; min_ix = i;
		}

		if (c->t < min_ty  && c->ny != 0) {
			min_ty = c->t; ny = c->ny; min_iy = i;
		}
	}

	

	if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);
}

CGameObject* CGameObject::AddAnimation(LPANIMATION anim)
{
	animations.push_back(anim);
	return this;
};

//trả về true nếu va chạm
bool CGameObject::AABB(float x1, float y1, float xx1, float yy1)
{
	float x2, y2, xx2, yy2;
	GetBoundingBox(x2, y2, xx2, yy2);

	float X1 = (x1 + xx1) / 2;
	float Y1 = (y1 + yy1) / 2;
	float W1 = abs(xx1 - x1) / 2;
	float H1 = abs(yy1 - y1) / 2;
	float X2 = (x2 + xx2) / 2;
	float Y2 = (y2 + yy2) / 2;
	float W2 = abs(xx2 - x2) / 2;
	float H2 = abs(yy2 - y2) / 2;
	return (abs(X2 - X1) <= W1 + W2 && abs(Y2 - Y1) <= H1 + H2);
}; 