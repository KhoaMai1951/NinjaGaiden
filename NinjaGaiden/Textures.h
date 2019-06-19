#pragma once
#include <unordered_map>
#include <d3dx9.h>

using namespace std;

enum ID_TEX
{
	ID_TEX_TEXT,
	ID_TEX_NINJA,
	ID_TEX_NINJA_ITEM,
	ID_TEX_NINJA_ENEMY,
	ID_TEX_BG_3_1,
	ID_TEX_BG_3_2,
	ID_TEX_BG_3_3,
	ID_TEX_TILESET1,
	ID_TEX_TILESET2,
	ID_TEX_TILESET3,
	ID_TEX_BAR,
	ID_TEX_BAR2
};


/*
	Manage texture database
*/
class CTextures
{
	static CTextures * __instance;

	unordered_map<int, LPDIRECT3DTEXTURE9> textures;

public:
	CTextures();
	void Add(int id, LPCSTR filePath, D3DCOLOR transparentColor = 0x000000);
	LPDIRECT3DTEXTURE9 Get(unsigned int i);

	static CTextures * GetInstance();
};