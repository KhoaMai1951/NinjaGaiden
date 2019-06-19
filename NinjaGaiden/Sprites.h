#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <map>
#include <vector>
#include <string>
using namespace std;

#define TILE_INDEX 30000
#define TEXT_INDEX 40000

class CSprite
{
	int id;				// Sprite ID in the sprite database
	int top;
	int left;
	int right; 
	int bottom;
	D3DXVECTOR2 origin;

	LPDIRECT3DTEXTURE9 texture;
public:
	CSprite(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9, float ox = 0, float oy = 0);

	void Draw(float x, float y, int alpha = 255, float scale_x = 1, float scale_y = 1);
};

typedef CSprite * LPSPRITE;

/*
	Manage sprite database
*/
class CSprites
{
	static CSprites* __instance; //<-- singleton
	map<int, LPSPRITE> sprites;

public:
	void Add(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex, int ox = 0, int oy = 0);
	LPSPRITE Get(int id);

	static CSprites* GetInstance();
};


class CAnimation
{
	int defaultTime, currentFrame, lastFrameTime;
	vector<LPSPRITE> frames;
	vector<DWORD> frame_time; 

public:
	bool loop, looped, finished; 
	CAnimation(int default_time = 100, bool loop = true)
	{
		this->defaultTime = default_time; 
		this->loop = loop;
		Restart();
	}
	CAnimation* Add(int spriteId, DWORD time = 0);
	void Render(float, float, int alpha = 255, float scale_x = 1, float scale_y = 1);
	void Restart() 
	{
		looped = false;
		finished = false;
		currentFrame = -1;
		lastFrameTime = -1;
	};
};
typedef CAnimation* LPANIMATION;

extern map<string, CAnimation*> Animations;