/* =============================================================
	INTRODUCTION TO GAME PROGRAMMING SE102

	SAMPLE 04 - COLLISION

	This sample illustrates how to:

		1/ Implement SweptAABB algorithm between moving objects
		2/ Implement a simple (yet effective) collision frame work

	Key functions:
		CGame::SweptAABB
		CGameObject::SweptAABBEx
		CGameObject::CalcPotentialCollisions
		CGameObject::FilterCollision

		CGameObject::GetBoundingBox

================================================================ */

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>


#include "Game.h"
#include "GameObject.h"
#include "Textures.h"

#include "Ryu.h"
#include "Wall.h"
#include "Stage3_1.h"
#include "Stage3_2.h"
#include "Stage3_3.h"


#define WINDOW_CLASS_NAME "SampleWindow"
#define MAIN_WINDOW_TITLE "Ninja Gaiden"

#define BACKGROUND_COLOR D3DCOLOR_XRGB(255, 255, 200)
#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 224

#define MAX_FRAME_RATE 120

CGame *game;
GameLevel* Stage = nullptr;

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

/*
	Load all game resources
	In this example: load textures, sprites, animations and mario object

	TO-DO: Improve this function by loading texture,sprite,animation,object from file
*/
void LoadResources()
{
	CTextures * textures = CTextures::GetInstance();

	textures->Add(ID_TEX_TEXT, "textures\\bitmap_font.png");
	textures->Add(ID_TEX_NINJA, "textures\\22708.png");
	textures->Add(ID_TEX_NINJA_ITEM, "textures\\22707.png");
	textures->Add(ID_TEX_NINJA_ENEMY, "textures\\23058.png");
	textures->Add(ID_TEX_BG_3_1, "textures\\NinjaGaidenMapStage3-1BG.png");
	textures->Add(ID_TEX_BG_3_2, "textures\\NinjaGaidenMapStage3-2BG.png");
	textures->Add(ID_TEX_BG_3_3, "textures\\NinjaGaidenMapStage3-3BG.png");
	textures->Add(ID_TEX_TILESET1, "textures\\NinjaGaidenMapStage3-1.png");
	textures->Add(ID_TEX_TILESET2, "textures\\NinjaGaidenMapStage3-2.png");
	textures->Add(ID_TEX_TILESET3, "textures\\NinjaGaidenMapStage3-3.png");
	textures->Add(ID_TEX_BAR, "textures\\blood_bar.png");
	textures->Add(ID_TEX_BAR2, "textures\\white_blood_bar.png");


	CSprites * sprites = CSprites::GetInstance();



	sprites->Add(10000, 2, 1, 19, 33, textures->Get(ID_TEX_NINJA), 8, 0); //ninja_idle
	sprites->Add(10001, 338, 2, 358, 33, textures->Get(ID_TEX_NINJA), 8, 0); //ninja_move
	sprites->Add(10002, 367, 2, 389, 33, textures->Get(ID_TEX_NINJA), 8, 0);
	sprites->Add(10003, 399, 2, 419, 33, textures->Get(ID_TEX_NINJA), 8, 0);
	sprites->Add(10004, 141, 49, 157, 71, textures->Get(ID_TEX_NINJA), 8, 0); //ninja_jump, ninja_hurt
	sprites->Add(10005, 165, 51, 187, 67, textures->Get(ID_TEX_NINJA), 11, -3);
	sprites->Add(10006, 193, 49, 209, 71, textures->Get(ID_TEX_NINJA), 8, 0);
	sprites->Add(10007, 216, 51, 238, 67, textures->Get(ID_TEX_NINJA), 11, -3);
	sprites->Add(10008, 41, 2, 59, 33, textures->Get(ID_TEX_NINJA), 8, -1);  //ninja_attack
	sprites->Add(10009, 65, 4, 105, 33, textures->Get(ID_TEX_NINJA), 8, -3); 
	sprites->Add(10010, 110, 4, 139, 33, textures->Get(ID_TEX_NINJA), 8, -3);
	sprites->Add(10100, 318, 79, 348, 117, textures->Get(ID_TEX_NINJA), 15, 19); //enemy_death

	sprites->Add(20000, 213, 57, 237, 89, textures->Get(ID_TEX_NINJA_ENEMY), 8, 0); //enemy swing
	sprites->Add(20001, 242, 57, 266, 89, textures->Get(ID_TEX_NINJA_ENEMY), 8, 0);
	sprites->Add(20002, 279, 49, 295, 89, textures->Get(ID_TEX_NINJA_ENEMY), 8, 8);
	sprites->Add(20003, 410, 11, 434, 43, textures->Get(ID_TEX_NINJA_ENEMY), 8, 0); //enemy throw
	sprites->Add(20004, 446, 11, 470, 43, textures->Get(ID_TEX_NINJA_ENEMY), 8, 0);
	sprites->Add(20005, 493, 0, 509, 43, textures->Get(ID_TEX_NINJA_ENEMY), 8, 11);
	sprites->Add(20006, 523, 6, 531, 21, textures->Get(ID_TEX_NINJA_ENEMY), 4, 8); //enemy throw-projectile
	sprites->Add(20007, 387, 65, 411, 90, textures->Get(ID_TEX_NINJA_ENEMY), 8, 0); //enemy gun
	sprites->Add(20008, 417, 61, 448, 90, textures->Get(ID_TEX_NINJA_ENEMY), 10, 4);
	sprites->Add(20009, 460, 67, 468, 71, textures->Get(ID_TEX_NINJA_ENEMY), 4, 2); //enemy gun-bullet
	sprites->Add(20010, 311, 76, 342, 90, textures->Get(ID_TEX_NINJA_ENEMY), 15, -12); //enemy cougar
	sprites->Add(20011, 352, 75, 376, 90, textures->Get(ID_TEX_NINJA_ENEMY), 12, -12);
	sprites->Add(20012, 475, 71, 491, 87, textures->Get(ID_TEX_NINJA_ENEMY), 8, 0); //enemy eagle
	sprites->Add(20013, 502, 56, 519, 81, textures->Get(ID_TEX_NINJA_ENEMY), 8, 16);
	sprites->Add(20014, 351, 250, 385, 298, textures->Get(ID_TEX_NINJA_ENEMY), 19, 20); //stage 3 boss
	sprites->Add(20015, 395, 244, 433, 298, textures->Get(ID_TEX_NINJA_ENEMY), 19, 27);
	sprites->Add(20016, 440, 270, 445, 275, textures->Get(ID_TEX_NINJA_ENEMY), 2, 2); //boss bomb
	sprites->Add(20017, 0, 57, 16, 89, textures->Get(ID_TEX_NINJA_ENEMY), 8, 0); //enymy run
	sprites->Add(20018, 21, 57, 37, 89, textures->Get(ID_TEX_NINJA_ENEMY), 8, 0);
	sprites->Add(20019, 383, 81, 415, 113, textures->Get(ID_TEX_NINJA), 16, 16); // nổ
	
	sprites->Add(00000, 65, 45, 80, 59, textures->Get(ID_TEX_NINJA_ITEM), 8, 7); //item carrier 1
	sprites->Add(00001, 88, 46, 100, 57, textures->Get(ID_TEX_NINJA_ITEM), 6, 6);
	sprites->Add(00002, 105, 44, 121, 59, textures->Get(ID_TEX_NINJA_ITEM), 6, 6); //item carrier 2
	sprites->Add(00003, 127, 43, 143, 58, textures->Get(ID_TEX_NINJA_ITEM), 8, 8);
	sprites->Add(00004, 0, 1, 15, 16, textures->Get(ID_TEX_NINJA_ITEM), 8, 8); //item power 5
	sprites->Add(00005, 20, 1, 35, 16, textures->Get(ID_TEX_NINJA_ITEM), 8, 8); //item power 10
	sprites->Add(00006, 39, 1, 55, 16, textures->Get(ID_TEX_NINJA_ITEM), 8, 8); //item point 500
	sprites->Add(00007, 60, 1, 76, 16, textures->Get(ID_TEX_NINJA_ITEM), 8, 8); //item point 1000

	sprites->Add(00020, 0, 0, 8, 16, textures->Get(ID_TEX_BAR), 4, 8); //blood bar
	sprites->Add(00030, 0, 0, 8, 16, textures->Get(ID_TEX_BAR2), 4, 8);  
	for (int i = 0; i < 36; i++)
		sprites->Add(TEXT_INDEX + i, i * 16, 0, i * 16 + 16, 16, textures->Get(ID_TEX_TEXT), 8, 8);

	Animations["ninja_idle"] = (new CAnimation(100))->Add(10000);
	Animations["ninja_move"] = (new CAnimation(100))->Add(10001)->Add(10002)->Add(10003);
	Animations["ninja_jump"] = (new CAnimation(60))->Add(10004)->Add(10005)->Add(10006)->Add(10007);
	Animations["ninja_hurt"] = (new CAnimation(5000))->Add(10004);
	Animations["ninja_attack"] = (new CAnimation(75, false))->Add(10008)->Add(10009)->Add(10010);

	Animations["enemy_death"] = (new CAnimation(100))->Add(10100);
	Animations["enemy_1"] = (new CAnimation(200))->Add(20000)->Add(20001)->Add(20002);
	Animations["enemy_2"] = (new CAnimation(200))->Add(20003)->Add(20004)->Add(20005);
	Animations["enemy_2_projectile"] = (new CAnimation(200))->Add(20006);
	Animations["enemy_3a"] = (new CAnimation(5000))->Add(20007);
	Animations["enemy_3b"] = (new CAnimation(5000))->Add(20008);
	Animations["enemy_3_bullet"] = (new CAnimation(5000))->Add(20009);
	Animations["enemy_4"] = (new CAnimation(100))->Add(20010)->Add(20011);

	Animations["carrier1"] = (new CAnimation(100))->Add(00000)->Add(00001);
	Animations["carrier2"] = (new CAnimation(200))->Add(00002)->Add(00003);
	Animations["item_power_5"] = (new CAnimation(200))->Add(00004);
	Animations["item_power_10"] = (new CAnimation(200))->Add(00005);
	Animations["item_point_500"] = (new CAnimation(200))->Add(00006);
	Animations["item_point_1000"] = (new CAnimation(200))->Add(00007);
	Animations["enemy_eagle"] = (new CAnimation(300))->Add(20012)->Add(20013);
	Animations["enemy_run"] = (new CAnimation(400))->Add(20017)->Add(20018);
	Animations["enemy_boss"] = (new CAnimation(400))->Add(20014);
	Animations["enemy_boss_die"] = (new CAnimation(400))->Add(20015);
	Animations["enemy_no"] = (new CAnimation(800))->Add(20019);
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd = CreateWindow(
		WINDOW_CLASS_NAME,
		MAIN_WINDOW_TITLE,
		WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		ScreenWidth,
		ScreenHeight,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (!hWnd)
	{
		OutputDebugString("[ERROR] CreateWindow failed");
		DWORD ErrCode = GetLastError();
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

void Run()
{
	MSG msg;
	int done = 0;
	DWORD frameStart = GetTickCount();
	DWORD tickPerFrame = 1000 / MAX_FRAME_RATE;
	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		DWORD now = GetTickCount();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		DWORD dt = min(now - frameStart, tickPerFrame * 2);

		if (dt >= tickPerFrame)
		{
			frameStart = now;
			game->ProcessKeyboard();
			Stage->Update(dt);
			Stage->Render();
		
			switch (Stage->next_level)
			{
			case STAGE_3_1:
				delete Stage;
				Stage = new Stage3_1();
				break;

			case STAGE_3_2:
				delete Stage;
				Stage = new Stage3_2();
				break;

			case STAGE_3_3:
				delete Stage;
				Stage = new Stage3_3();
				break;
			}
		}
		else
			Sleep(tickPerFrame - dt);
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	game = CGame::GetInstance();
	game->Init(hWnd);
	game->InitKeyboard();
	SetWindowPos(hWnd, 0, 200, 40, SCREEN_WIDTH * 3, SCREEN_HEIGHT * 3, SWP_NOOWNERZORDER | SWP_NOZORDER);


	LoadResources();
	Stage = new Stage3_1();

	//Stage = new Stage3_2();

	//Stage = new Stage3_3();


	Run();

	return 0;
}