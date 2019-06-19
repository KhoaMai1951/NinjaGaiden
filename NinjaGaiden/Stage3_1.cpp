#include "Stage3_1.h"

#include "Enemy.h"
#include "EnemyGun.h"
#include "EnemyAnimal.h"
#include "EnemyEagle.h"
#include "EnemyThrow.h"
#include "ItemCarrier.h"
#include "EnemyRun.h"

#include "Wall.h"
#include "WallAI.h"


Stage3_1::Stage3_1()
 {
	load_tile_map(CTextures::GetInstance()->Get(ID_TEX_TILESET1), "map/NinjaGaidenMapStage3-1.txt");


	vector<int> arr = GameLevel::read_map("map/NinjaGaidenMapStage3-1BG.txt");
	
	//
	GameObjects.push_back((new EnemyAnimal(915, 75, -1)));
	GameObjects.push_back((new EnemyAnimal(935, 77,- 1)));


	//GameObjects.push_back((new EnemyThrow(100, 0, -1)));
	while (arr.size() >= 8)
	{
		switch (arr[1])
		{
		case 0:
		{
			GameObjects.push_back(new WallAI(arr[3], arr[4], arr[5], arr[6]));

			break;
		}
		case 1:
		{
			GameObjects.push_back(new CWall(arr[3], arr[4], arr[5], arr[6]));



			break;
		}
		case 2:
		{
			GameObjects.push_back((new Enemy(arr[3], arr[4], arr[7] ? -1 : 1))->AddAnimation(Animations["enemy_1"]));

			break;
		}
		case 3:
		{
			GameObjects.push_back(new ItemCarrier(arr[3], arr[4], Animations["carrier1"]));


			break;
		}
		case 4:
		{
			GameObjects.push_back((new EnemyThrow(arr[3], arr[4], arr[7] ? -1 : 1)));

			break;
		}
		case 5:
		{
			GameObjects.push_back((new EnemyGun(arr[3], arr[4], arr[7] ? -1 : 1)));


			break;
		}
		case 6:
		{
			GameObjects.push_back((new EnemyAnimal(arr[3], arr[4], arr[7] ? -1 : 1)));
			break;
		}
		case 7:
		{
			GameObjects.push_back((new EnemyEagle(arr[3], arr[4], arr[7] ? -1 : 1)));
			break;
		}
		case 8:
		{
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

};

void Stage3_1::Update(DWORD dt)
{

	SpatialGrid* grid = SpatialGrid::GetInstance();
	grid->Clear();
	for (unsigned i = 0; i < GameObjects.size(); i++)
		grid->Insert(GameObjects[i]);
	for (unsigned i = 0; i < GameObjects.size(); i++)
		GameObjects[i]->Update(dt);
	if (ryu->x > 1968)
		next_level = STAGE_3_2;
	else if (ryu->HP == 0 || ryu->y > 256)
			next_level = STAGE_3_1;
	
	for (int i = GameObjects.size() - 1; i >= 0; --i)
		if (GameObjects[i]->is_dead)
		{
			delete GameObjects[i];
			GameObjects.erase(GameObjects.begin() + i);
		}

	Camera::GetInstance()->Update(max(min(ryu->x, 1920), 128), 96); 
	Camera::GetInstance()->Transform(CGame::GetInstance()->GetDirect3DDevice()); 
};

void Stage3_1::Render()
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
		stringstream ss, ss1;
		ss.str(std::string());
		ss << "SCORE  " << ryu->score;
		CGameObject::DrawText(ss.str(), camera->left + 8, 0); //vẽ điểm (score)
		CGameObject::DrawText("STAGE 3 1", camera->left + 110, 0); //vẽ tên màn
		
		CGameObject::DrawText("NINJA", camera->left + 110, 16);
		for (int i = 0; i < ryu->HP; i++)
		{
			CSprites::GetInstance()->Get(00020)->Draw((camera->left + 160 )+ i*6, 16, 255, 0.5f, 0.5f);
		}
		for (int i = 0; i < 16 - ryu->HP; i++)
		{
			int temp= camera->left + 160 + ryu->HP *6;
			CSprites::GetInstance()->Get(00030)->Draw(temp+i*6, 16, 255, 0.5f, 0.5f);
		}
		d3ddv->EndScene();
	}

	// Display back buffer content to the screen
	d3ddv->Present(NULL, NULL, NULL, NULL);
};