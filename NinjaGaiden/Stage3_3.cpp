#include "Stage3_3.h"


#include "Enemy.h"
#include "EnemyGun.h"
#include "EnemyAnimal.h"
#include "EnemyThrow.h"
#include "ItemCarrier.h"
#include "EnemyBoss.h"

#include "Wall.h"
#include "Tile.h"
#include "WallAI.h"

#include "Camera.h"
#include "Textures.h"
#include "SpatialGrid.h"


Stage3_3::Stage3_3()
{

	LPDIRECT3DTEXTURE9 tile_set = CTextures::GetInstance()->Get(ID_TEX_TILESET3);
	CSprites * sprites = CSprites::GetInstance();


	vector<int> arr = GameLevel::read_map("map/NinjaGaidenMapStage3-3.txt");

	int tile_set_count = arr[0];
	int tile_map_row = arr[1];
	int tile_map_column = arr[2];
	int tile_height = arr[3];
	int tile_width = arr[4];
	int tile_set_row = arr[5];
	int tile_set_column = arr[6];

	arr.erase(arr.begin(), arr.begin() + 6);
	for (int i = 0; i < tile_set_count; i++)
	{
		int l = (i % tile_set_column) * tile_width;
		int t = (i / tile_set_column) * tile_height;
		int r = l + tile_width;
		int b = t + tile_height;
		sprites->Add(30000 + i, l, t, r, b, tile_set);
	}

	for (unsigned i = 0; i < arr.size(); i++)
	{
		int l = ((i - 1) % tile_map_column) * tile_width;
		int t = (i / tile_map_column) * tile_height;
		Tiles.push_back(new Tile(l, t, sprites->Get(30000 + arr[i])));
	}



	
	arr = GameLevel::read_map("map/NinjaGaidenMapStage3-3BG.txt");

	
	GameObjects.push_back((new EnemyBoss(570, 100, -1)));
	while (arr.size() >= 8)
	{
		switch (arr[1])
		{
		case 0:
		{
			GameObjects.push_back(new CWall(arr[3], arr[4], arr[5], arr[6]));
			break;
		}

		case 3:
		{
			/*GameObjects.push_back((new Enemy(arr[3], arr[4], arr[7] ? -1 : 1))->AddAnimation(Animations["enemy_1"]));*/
			break;
		}
		case 4:
		{
			/*GameObjects.push_back((new Enemy(arr[3], arr[4], arr[7] ? -1 : 1))->AddAnimation(Animations["enemy_2"]));
			GameObjects.push_back(new WallAI(arr[3], arr[4], arr[5], arr[6]));*/
			break;
		}
		case 5:
		{
			//.push_back(new ItemCarrier(arr[3], arr[4], Animations["carrier1"]));
			break;
		}
		case 6:
		{

			break;
		}


		default:
		{
			//GameObjects.push_back(new CWall(arr[3], arr[4], arr[5], arr[6]));

		}
		}

		arr.erase(arr.begin(), arr.begin() + 8);
	}


	ryu = new CRyu();


	ryu->SetPosition(590.0f, 0);
	GameObjects.push_back(ryu);


	SpatialGrid::GetInstance()->SetCell(256);

	Camera::GetInstance()->Update(640,65);
	Camera::GetInstance()->Transform(CGame::GetInstance()->GetDirect3DDevice());
};
//
void Stage3_3::Update(DWORD dt)
{

	SpatialGrid* grid = SpatialGrid::GetInstance();
	grid->Clear();
	for (unsigned i = 0; i < GameObjects.size(); i++)
		grid->Insert(GameObjects[i]);
	for (unsigned i = 0; i < GameObjects.size(); i++)
		GameObjects[i]->Update(dt);
	for (int i = GameObjects.size() - 1; i >= 0; --i)
		if (GameObjects[i]->is_dead)
		{
			delete GameObjects[i];
			GameObjects.erase(GameObjects.begin() + i);
		}

};

void Stage3_3::Render()
{
	CGame* game = CGame::GetInstance();
	Camera* camera = Camera::GetInstance();
	LPDIRECT3DDEVICE9 d3ddv = game->GetDirect3DDevice();
	LPDIRECT3DSURFACE9 bb = game->GetBackBuffer();
	LPD3DXSPRITE spriteHandler = game->GetSpriteHandler();
	if (d3ddv->BeginScene())
	{
		// Clear back buffer with a color
		d3ddv->ColorFill(bb, NULL, 0x00000000);
		for (unsigned i = 0; i < Tiles.size(); i++)
			Tiles[i]->Render();
		for (unsigned i = 0; i < GameObjects.size(); i++)
			GameObjects[i]->Render();
		stringstream ss;
		ss.str(std::string());
		ss << "SCORE  " << ryu->score;
		CGameObject::DrawText(ss.str().c_str(), camera->left + 8, -32);
		CGameObject::DrawText("STAGE 3 3", camera->left + 110, -32);

		CGameObject::DrawText("NINJA", camera->left + 110, -16);
		for (int i = 0; i < ryu->HP; i++)
		{
			CSprites::GetInstance()->Get(00020)->Draw((camera->left + 160) + i * 6, -16, 255, 0.5f, 0.5f);
		}
		for (int i = 0; i < 16 - ryu->HP; i++)
		{
			int temp = camera->left + 160 + ryu->HP * 6;
			CSprites::GetInstance()->Get(00030)->Draw(temp + i * 6, -16, 255, 0.5f, 0.5f);
		}
		d3ddv->EndScene();
	}

	// Display back buffer content to the screen
	d3ddv->Present(NULL, NULL, NULL, NULL);
};