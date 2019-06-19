#include "EnemyEagle.h"
#include "Ryu.h"



EnemyEagle::EnemyEagle(float x, float y, int facing) : Enemy(x, y, facing)
{
	AddAnimation(Animations["enemy_eagle"]);
	state = 0;
}

void EnemyEagle::Update(DWORD dt)
{
	switch (state)
	{
		case 0:
			if (IsInCamera())
				state = 1;
			break;

		case 1:
			CGameObject::Update(dt);
			x += dx;
			y += dy;
			for (CGameObject* obj : GameObjects)
				if (dynamic_cast<CRyu*>(obj))
				{
					float angle = 0;
					angle = xy2Dir(obj->x - x, obj->y - y);
					//tính góc so với Ryu
					Dir2xy(0.1, angle, vx, vy);
					facing = obj->x - x > 0 ? 1 : -1;
					break;
				}
			break;
	}
}

