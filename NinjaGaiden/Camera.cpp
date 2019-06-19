#include "Camera.h"

Camera* Camera::instance = NULL;

Camera::Camera()
{
	cameraDimension.x = 256;
	cameraDimension.y = 224;
	top = 0;
	left = 0;
	right = cameraDimension.x;
	bottom = cameraDimension.y;
	D3DXMatrixOrthoLH(&orthoResourceMatrix, cameraDimension.x, -cameraDimension.y, 0.0f, 1.0f);
	D3DXMatrixIdentity(&identityMatrix);
	viewMatrix = D3DXMATRIX(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		-cameraDimension.x / 2, -cameraDimension.x / 2, 0, 1);
};

void Camera::Update(float x, float y)
{
	top = y - cameraDimension.y / 2;
	left = x - cameraDimension.x / 2;
	right = left + cameraDimension.x;
	bottom = top + cameraDimension.y;
	viewMatrix = D3DXMATRIX(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		-x, -y, 0, 1);
};

void Camera::Transform(LPDIRECT3DDEVICE9 d3ddv)
{
	d3ddv->SetTransform(D3DTS_PROJECTION, &orthoResourceMatrix);
	d3ddv->SetTransform(D3DTS_WORLD, &identityMatrix);
	d3ddv->SetTransform(D3DTS_VIEW, &viewMatrix);
};

Camera* Camera::GetInstance()
{
	if (instance == NULL)
		instance = new Camera();
	return instance;
}
