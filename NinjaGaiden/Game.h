#pragma once
#define DIRECTINPUT_VERSION 0x0800
#define KEYBOARD_BUFFER_SIZE 1024
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include <dinput.h>
#include <algorithm>


class CGame
{
	static CGame * __instance;
	HWND hWnd;									// Window handle

	LPDIRECT3D9 d3d;						// Direct3D handle
	LPDIRECT3DDEVICE9 d3ddv;				// Direct3D device object

	LPDIRECT3DSURFACE9 backBuffer;
	LPD3DXSPRITE spriteHandler;			// Sprite helper library to help us draw 2D image on the screen 

	LPDIRECTINPUT8       di;		// The DirectInput object         
	LPDIRECTINPUTDEVICE8 didv;		// The keyboard device 

	BYTE  keyStates[256];			// DirectInput keyboard state buffer 
	BYTE  prevStates[256];			// DirectInput keyboard state buffer 

public:
	CGame();
	void InitKeyboard();
	void Init(HWND hWnd);

	//
	void Draw(float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha, float scale_x, float scale_y, D3DXVECTOR2 origin = D3DXVECTOR2(0, 0));
	//origin: điểm neo trong sprite
	bool IsKeyDown(int KeyCode);
	bool IsKeyPress(int KeyCode);
	void ProcessKeyboard();

	static void SweptAABB(
		float ml,			// move left 
		float mt,			// move top
		float mr,			// move right 
		float mb,			// move bottom
		float dx,			// khoảng cách đi
		float dy,			// khoảng cách đi
		float sl,			// static left
		float st,			// static top 
		float sr,			// static right 
		float sb,			// static bottom
		float &t,			// thời gian va chạm
		float &nx,			// chiều va chạm
		float &ny);			// chiều va chạm

	LPDIRECT3DDEVICE9 GetDirect3DDevice() { return this->d3ddv; }
	LPDIRECT3DSURFACE9 GetBackBuffer() { return backBuffer; }
	LPD3DXSPRITE GetSpriteHandler() { return this->spriteHandler; }

	static CGame* GetInstance();

	~CGame();
};

