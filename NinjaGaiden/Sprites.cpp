#include "Sprites.h"
#include "Game.h"

CSprite::CSprite(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 texture, float ox, float oy)
{
	this->id = id;
	this->top = top;
	this->left = left;
	this->right = right;
	this->bottom = bottom;
	this->texture = texture;
	origin.x = ox;
	origin.y = oy;
}

void CSprite::Draw(float x, float y, int alpha, float scale_x, float scale_y)
{
	CGame::GetInstance()->Draw(x, y, texture, left, top, right, bottom, alpha, scale_x, scale_y, origin);
}

CSprites* CSprites::__instance = NULL;

CSprites* CSprites::GetInstance()
{
	if (__instance == NULL)
		__instance = new CSprites();
	return __instance;
}

void CSprites::Add(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex, int ox, int oy)
{
	LPSPRITE s = new CSprite(id, left, top, right, bottom, tex, ox, oy);
	sprites[id] = s;
}

LPSPRITE CSprites::Get(int id)
{
	return sprites[id];
}

CAnimation* CAnimation::Add(int spriteId, DWORD time)
{
	if (time == 0)
		time = this->defaultTime;

	frames.push_back(CSprites::GetInstance()->Get(spriteId));
	frame_time.push_back(time);

	return this;
}

void CAnimation::Render(float x, float y, int alpha, float scale_x, float scale_y)
{
	DWORD now = GetTickCount();
	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else if (!finished)
	{
		DWORD t = frame_time[currentFrame];
		if (now - lastFrameTime > t)
		{
			++currentFrame;
			lastFrameTime = now;
			if (currentFrame == frames.size())
			{
				if (loop)
				{
					currentFrame = 0;
					looped = true;
				}
				else
				{
					currentFrame = frames.size() - 1;
					finished = true;
				}
			}
			else
				looped = false;
		}
	}
	frames[currentFrame]->Draw(x, y, alpha, scale_x, scale_y);
}



map<string, LPANIMATION> Animations;