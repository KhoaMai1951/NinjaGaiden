#include "Game.h"

CGame * CGame::__instance = NULL;

/*
	Initialize DirectX, create a Direct3D device for rendering within the window, initial Sprite library for
	rendering 2D images
	- hInst: Application instance handle
	- hWnd: Application window handle
*/
CGame::CGame()
{
	LPDIRECT3D9 d3d = NULL;						// Direct3D handle
	LPDIRECT3DDEVICE9 d3ddv = NULL;				// Direct3D device object

	LPDIRECT3DSURFACE9 backBuffer = NULL;
	LPD3DXSPRITE spriteHandler = NULL;
}

void CGame::Init(HWND hWnd)
{
	LPDIRECT3D9 d3d = Direct3DCreate9(D3D_SDK_VERSION);

	this->hWnd = hWnd;

	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount = 1;

	RECT r;
	GetClientRect(hWnd, &r);	// retrieve Window width & height 

	d3dpp.BackBufferHeight = r.bottom;
	d3dpp.BackBufferWidth = r.right;

	d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3ddv);

	if (d3ddv == NULL)
	{
		OutputDebugString("[ERROR] CreateDevice failed\n");
		return;
	}

	d3ddv->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);

	// Initialize sprite helper from Direct3DX helper library
	D3DXCreateSprite(d3ddv, &spriteHandler);

	OutputDebugString("[INFO] InitGame done;\n");
}

/*
	Utility function to wrap LPD3DXSPRITE::Draw
*/
void CGame::Draw(float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha, float scale_x, float scale_y, D3DXVECTOR2 origin)
{
	RECT r;
	r.top = top;
	r.left = left;
	r.right = right;
	r.bottom = bottom;

	D3DXMATRIX mat;
	D3DXVECTOR2 pos(x, y);
	D3DXVECTOR2 scale(scale_x, scale_y);
	D3DXMatrixTransformation2D(&mat, &origin, 0, &scale, 0, 0, &(pos - origin));

	spriteHandler->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_OBJECTSPACE | D3DXSPRITE_DONOTSAVESTATE);
	spriteHandler->SetTransform(&mat);
	spriteHandler->Draw(texture, &r, NULL, 0, D3DCOLOR_ARGB(alpha, 255, 255, 255));
	spriteHandler->End();
}

bool CGame::IsKeyDown(int KeyCode)
{
	return (keyStates[KeyCode] & 0x80) > 0;
}

bool CGame::IsKeyPress(int KeyCode)
{
	return !((prevStates[KeyCode] & 0x80) > 0) && ((keyStates[KeyCode] & 0x80) > 0);
}

void CGame::InitKeyboard()
{

	if (DirectInput8Create((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&di, 0) != DI_OK)
	{
		return;
	}

	// TO-DO: put in exception handling
	if (di->CreateDevice(GUID_SysKeyboard, &didv, NULL) != DI_OK)
	{
		return;
	}

	// Set the data format to "keyboard format" - a predefined data format 
	//
	// A data format specifies which controls on a device we
	// are interested in, and how they should be reported.
	//
	// This tells DirectInput that we will be passing an array
	// of 256 bytes to IDirectInputDevice::GetDeviceState.

	didv->SetDataFormat(&c_dfDIKeyboard);
	didv->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

}

void CGame::ProcessKeyboard()
{
	HRESULT hr;

	// Collect all key states first
	for (int i = 0; i < 256; ++i)
		prevStates[i] = keyStates[i];
	hr = didv->GetDeviceState(sizeof(keyStates), keyStates);
	if (FAILED(hr))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			if (didv->Acquire() == DI_OK)
			{
			}
			else return;
		}
		else
		{
			return;
		}
	}
}

CGame::~CGame()
{
	if (spriteHandler != NULL) spriteHandler->Release();
	if (backBuffer != NULL) backBuffer->Release();
	if (d3ddv != NULL) d3ddv->Release();
	if (d3d != NULL) d3d->Release();
}

/*
	SweptAABB
*/

//giá trị trả về 1 cho biết không có khả năng va chạm
void CGame::SweptAABB(
	float ml, float mt, float mr, float mb, float dx, float dy, //<--
	float sl, float st, float sr, float sb, float &t, float &nx, float &ny) //<-- t thoi gian va cham, nx chieu va cham phuong ngang
{

	float dx_entry, dx_exit, tx_entry, tx_exit;
	// Khoảng cách gần nhất và xa nhất giữa các cạnh của 2 hìnhh
	// dx_entry là khoảng cách gần nhất, dx_exit là khoảng cách xa nhất
	float dy_entry, dy_exit, ty_entry, ty_exit;

	float t_entry;
	float t_exit;

	t = -1.0f;			// no collision
	nx = ny = 0;

	//
	// Broad-phase test 
	//

	float bl = dx > 0 ? ml : ml + dx;
	float bt = dy > 0 ? mt : mt + dy;
	float br = dx > 0 ? mr + dx : mr;
	float bb = dy > 0 ? mb + dy : mb;

	if (br < sl || bl > sr || bb < st || bt > sb) return;

	if (dx == 0 && dy == 0) return;		// moving object is not moving > obvious no collision

	if (dx > 0)
	{
		dx_entry = sl - mr;
		dx_exit = sr - ml;
	}
	else if (dx < 0)
	{
		dx_entry = sr - ml;
		dx_exit = sl - mr;
	}


	if (dy > 0)
	{
		dy_entry = st - mb;
		dy_exit = sb - mt;
	}
	else if (dy < 0)
	{
		dy_entry = sb - mt;
		dy_exit = st - mb;
	}

	if (dx == 0) //Trường hợp vận tốc là 0 thì các giá trị là vô cực
	{
		tx_entry = -std::numeric_limits<float>::infinity();
		tx_exit = std::numeric_limits<float>::infinity();
	}
	else
	{
		tx_entry = dx_entry / dx; //Khoảng thời gian bắt đầu va chạm theo x
		tx_exit = dx_exit / dx; //khoảng thời gian kết thúc va chạm
	}

	if (dy == 0)
	{
		ty_entry = -std::numeric_limits<float>::infinity();
		ty_exit = std::numeric_limits<float>::infinity();
	}
	else
	{
		ty_entry = dy_entry / dy;
		ty_exit = dy_exit / dy;
	}


	if ((tx_entry < 0.0f && ty_entry < 0.0f) || tx_entry > 1.0f || ty_entry > 1.0f)
		return;

	t_entry = max(tx_entry, ty_entry);
	t_exit = min(tx_exit, ty_exit);

	if (t_entry > t_exit)
		return;

	t = t_entry;

	if (tx_entry > ty_entry)
	{
		ny = 0.0f;
		dx > 0 ? nx = -1.0f : nx = 1.0f;
	}
	else
	{
		nx = 0.0f;
		dy > 0 ? ny = -1.0f : ny = 1.0f;
	}
}

CGame *CGame::GetInstance()
{
	if (__instance == NULL) __instance = new CGame();
	return __instance;
}