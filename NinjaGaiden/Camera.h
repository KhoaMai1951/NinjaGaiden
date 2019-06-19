#pragma once
#include "Game.h"

class Camera
{
	static Camera* instance; //<-- singleton

protected:
	D3DXVECTOR2 cameraDimension;
	D3DXMATRIX orthoResourceMatrix;
	D3DXMATRIX identityMatrix;
	D3DXMATRIX viewMatrix;

public:
	float top, left, right, bottom; 
	Camera();
	float cameraWidth() { return cameraDimension.x; };
	float cameraHeight() { return cameraDimension.y; };
	void Update(float, float);
	void Transform(LPDIRECT3DDEVICE9);
	void setCameraDimension(float x, float y) { cameraDimension.x = x; cameraDimension.y = y; }
	static Camera* GetInstance();
};

