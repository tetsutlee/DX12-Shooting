#pragma once
#include"Input.h"
#include"DirectX12.h"
#include"Mesh.h"
#include"Sprite.h"
#include"GameFunction.h"

class Player
{
public:
	Player(DirectX12* dx12, Input* input);
	~Player();
	void Initialize(WNDCLASSEX& wndc, HWND& hwnd, const int width, const int height);
	void Update(const int width, const int height);
	void Draw();

private:
	DirectX12* dx12;
	Input* input;
	Mesh* mesh[5];

	XMFLOAT3 scale;
	XMFLOAT3 rotation;
	XMFLOAT3 bulletScale;
	XMFLOAT3 bulletRot;

	bool launchSwitch;
	bool startMotion;
	bool endMotion;
	bool isCollision[5];
	short time;

public:
	short life;
	XMFLOAT3 position;
	XMFLOAT3 bulletPos;
};

