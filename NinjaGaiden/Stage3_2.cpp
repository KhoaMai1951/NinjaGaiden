#include "Stage3_2.h"

#include "Enemy.h"
#include "EnemyGun.h"
#include "EnemyAnimal.h"
#include "EnemyThrow.h"
#include "ItemCarrier.h"

#include "Wall.h"
#include "WallAI.h"
#include "EnemyEagle.h"
#include "EnemyRun.h"

Stage3_2::Stage3_2()
{
	load_tile_map(CTextures::GetInstance()->Get(ID_TEX_TILESET2), "map/NinjaGaidenMapStage3-2.txt");

	vector<int> arr = GameLevel::read_map("map/NinjaGaidenMapStage3-2BG.txt");


	while (arr.size() >= 8)
	{
		switch (arr[1])
		{
		case 0:
		{
			GameObjects.push_back(new CWall(arr[3], arr[4], arr[5], arr[6]));
			break;
		}
		case 1:
		{
			GameObjects.push_back(new WallAI(arr[3], arr[4], arr[5], arr[6]));
			break;
		}
		case 2:
		{
			GameObjects.push_back((new Enemy(arr[3], arr[4], arr[7] ? -1 : 1))->AddAnimation(Animations["enemy_1"]));
			break;
		}
		case 3:
		{
			GameObjects.push_back((new EnemyGun(arr[3], arr[4], arr[7] ? -1 : 1)));
			//GameObjects.push_back((new Enemy(arr[3], arr[4], arr[7] ? -1 : 1))->AddAnimation(Animations["enemy_1"]));
			break;
		}
		case 4:
		{
			GameObjects.push_back((new EnemyThrow(arr[3], arr[4], arr[7] ? -1 : 1)));
			/*GameObjects.push_back((new Enemy(arr[3], arr[4], arr[7] ? -1 : 1))->AddAnimation(Animations["enemy_2"]));
			GameObjects.push_back(new WallAI(arr[3], arr[4], arr[5], arr[6]));*/
			break;
		}
		case 5:
		{
			GameObjects.push_back((new EnemyAnimal(arr[3], arr[4], arr[7] ? -1 : 1)));
			/*GameObjects.push_back(new ItemCarrier(arr[3], arr[4], Animations["carrier1"]));
			break;*/
			break;
		}
		case 6:
		{
			GameObjects.push_back(new ItemCarrier(arr[3], arr[4], Animations["carrier2"]));

			break;
		}
		case 7:
		{
			GameObjects.push_back((new EnemyEagle(arr[3], arr[4], arr[7] ? -1 : 1)));
			break;
		}
		case 8:
		{
			GameObjects.push_back((new EnemyRun(arr[3], arr[4], arr[7] ? -1 : 1))->AddAnimation(Animations["enemy_run"]));
			break;
		}
		case 9:
		{
			break;
		}
		case 10:
		{
			break;
		}

		default:
		{


		}
		}

		arr.erase(arr.begin(), arr.begin() + 8);
	}
	ryu = new CRyu();


	ryu->SetPosition(50.0f, 0);
	GameObjects.push_back(ryu);

	SpatialGrid::GetInstance()->SetCell(256);
}
void Stage3_2::Update(DWORD dt)
{

	SpatialGrid* grid = SpatialGrid::GetInstance();
	grid->Clear();
	for (unsigned i = 0; i < GameObjects.size(); i++)
		grid->Insert(GameObjects[i]);
	for (unsigned i = 0; i < GameObjects.size(); i++)
		GameObjects[i]->Update(dt);
	if (ryu->x > 2988)
		next_level = STAGE_3_3;
	else if (ryu->y > 256)
		next_level = STAGE_3_2;
	for (int i = GameObjects.size() - 1; i >= 0; --i)
		if (GameObjects[i]->is_dead)
		{
			delete GameObjects[i];
			GameObjects.erase(GameObjects.begin() + i);
		}

	Camera::GetInstance()->Update(max(min(ryu->x, 2940), 128), 96);
	Camera::GetInstance()->Transform(CGame::GetInstance()->GetDirect3DDevice());
};
void Stage3_2::Render()
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
		CGameObject::DrawText(ss.str().c_str(), camera->left + 8, 0);
		CGameObject::DrawText("STAGE 3 2", camera->left + 110, 0);


		CGameObject::DrawText("NINJA", camera->left + 110, 16);
		for (int i = 0; i < ryu->HP; i++)
		{
			CSprites::GetInstance()->Get(00020)->Draw((camera->left + 160) + i * 6, 16, 255, 0.5f, 0.5f);
		}
		for (int i = 0; i < 16 - ryu->HP; i++)
		{
			int temp = camera->left + 160 + ryu->HP * 6;
			CSprites::GetInstance()->Get(00030)->Draw(temp + i * 6, 16, 255, 0.5f, 0.5f);
		}
		d3ddv->EndScene();
	}

	// Display back buffer content to the screen
	d3ddv->Present(NULL, NULL, NULL, NULL);
};
