#include "SpatialGrid.h"
#include <sstream>

SpatialGrid* SpatialGrid::instance = nullptr;

SpatialGrid* SpatialGrid::GetInstance()
{
	if (instance == nullptr)
		instance = new SpatialGrid();
	return instance;
};

void SpatialGrid::Clear()
{
	map<string, vector<CGameObject*>>::iterator it;
	for (it = Grid.begin(); it != Grid.end(); ++it)
		it->second.clear();
	Grid.clear();
};

void SpatialGrid::SetCell(int cell_size)
{
	Clear();
	CellSize = cell_size;
};


void SpatialGrid::Index(CGameObject* object, float& x1, float& y1, float& x2, float& y2)
{
	object->GetBoundingBox(x1, y1, x2, y2);
	x1 = floor(x1 / CellSize);
	y1 = floor(y1 / CellSize);
	x2 = ceil(x2 / CellSize);
	y2 = ceil(y2 / CellSize);
};


void SpatialGrid::Insert(CGameObject* object)
{
	float x1, y1, x2, y2;
	float i, j;
	Index(object, x1, y1, x2, y2);
	stringstream ss;
	for (i = x1; i < x2; ++i)
		for (j = y1; j < y2; ++j)
			Grid[IndexMakeKey(ss, i, j)].push_back(object);
};

set<CGameObject*> SpatialGrid::Get(float x1, float y1, float x2, float y2) //lấy vật thể thuộc tọa độ của lưới
{
	x1 = floor(x1 / CellSize);
	y1 = floor(y1 / CellSize);
	x2 = ceil(x2 / CellSize);
	y2 = ceil(y2 / CellSize);
	set<CGameObject*> result;
	stringstream ss;
	string key;
	for (float i = x1; i < x2; ++i)
		for (float j = y1; j < y2; ++j)
		{
			key = IndexMakeKey(ss, i, j);
			vector<CGameObject*> v_cell = Grid[key];
			result.insert(v_cell.begin(), v_cell.end());

		}
	return result;
};

string SpatialGrid::IndexMakeKey(stringstream& ss, float& x, float& y)
{
	ss.str(std::string());
	ss << x << "_" << y;
	return ss.str();
};