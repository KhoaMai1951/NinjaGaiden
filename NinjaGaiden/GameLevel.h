#pragma once
#include <vector>
#include <fstream>
#include <sstream>

#include "Ryu.h"
#include "Tile.h"
#include "Game.h"
#include "Camera.h"
#include "Sprites.h"
#include "Textures.h"
#include "SpatialGrid.h"

using namespace std;

enum LEVEL
{
	STAGE_3_1,
	STAGE_3_2,
	STAGE_3_3,
};

class GameLevel
{
protected:
	int tile_count; //số lượng tile
	CRyu* ryu;
	vector<CGameObject*> Tiles; //chứa tile

	vector<int> read_map(string file_path)
	{
		ifstream map_file(file_path);
		vector<int> result;
		if (map_file.is_open())
		{
			while (true)
			{
				int x;
				map_file >> x;
				if (map_file.eof())
					break;
				result.push_back(x);
			}
			map_file.close();
		}
		return result;
	};

	void load_tile_map(LPDIRECT3DTEXTURE9 tile_set, string file_path)
	{
		CSprites * sprites = CSprites::GetInstance();
		vector<int> arr = GameLevel::read_map(file_path);

		int tile_set_count = arr[0];//350
		int tile_map_row = arr[1]; //13
		int tile_map_column = arr[2];//160
		int tile_height = arr[3];//16
		int tile_width = arr[4];//16
		int tile_set_row = arr[5];//4
		int tile_set_column = arr[6];//88

		tile_count = tile_set_count;

		arr.erase(arr.begin(), arr.begin() + 6);

		for (int i = 0; i < tile_set_count; i++)
		{
			int l = (i % tile_set_column) * tile_width;
			int t = (i / tile_set_column) * tile_height;
			int r = l + tile_width;
			int b = t + tile_height;
			sprites->Add(TILE_INDEX + i, l, t, r, b, tile_set);
		}

		for (unsigned i = 0; i < arr.size(); i++)
		{
			int l = ((i - 1) % tile_map_column) * tile_width;
			int t = (i / tile_map_column) * tile_height;
			Tiles.push_back(new Tile((float)l, (float)t, sprites->Get(TILE_INDEX + arr[i])));
		}
	};

public:
	int next_level;

	GameLevel()
	{
		next_level = -1;
		tile_count = 0;
	};
	virtual void Update(DWORD) {};
	virtual void Render() {};

	virtual ~GameLevel()
	{
		for (int i = GameObjects.size() - 1; i >= 0; --i)
			delete GameObjects[i];
		GameObjects.clear();
		SpatialGrid::GetInstance()->Clear();
		for (int i = Tiles.size() - 1; i >= 0; --i)
			delete Tiles[i];

		CSprites* sprites = CSprites::GetInstance();
		for (int i = 0; i < tile_count; i++)
			delete (sprites->Get(TILE_INDEX + i));
	};
};