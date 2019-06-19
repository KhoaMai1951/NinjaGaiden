#include "GameObject.h"

class Enemy_Bullet : public CGameObject
{
protected:
	RECT bbox;

public:
	Enemy_Bullet(float, float, float, float, CAnimation*);
	void Update(DWORD dt);
	void Render();
	void GetBoundingBox(float&, float&, float&, float&);
	bool IsInCamera();
};