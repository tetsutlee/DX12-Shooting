#pragma once
#include"DirectX12.h"
#include"Input.h"
#include<DirectXMath.h>
using namespace DirectX;

class Camera
{
public:
	Camera(Input* input);
	~Camera();
	void Update();
	XMFLOAT3 position;
	XMFLOAT3 target;
	XMFLOAT3 upVector;
	XMMATRIX matView;

private:
	Input* input;	
	float angle;//Camera angle
};

