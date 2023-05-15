#pragma once
#include"Input.h"
#include"DirectX12.h"
#include"Mesh.h"
#include"GameFunction.h"
#include"Player.h"

class Enemy
{
public:
	Enemy(DirectX12* dx12, Input* input, Player* player);
	~Enemy();
	void Initialize(WNDCLASSEX& wndc, HWND& hwnd, const int width, const int height);
	void Update(const int width, const int height);
	void Draw();

private:
	DirectX12* dx12;
	Input* input;
	Mesh* mesh[10];
	Player* player;
	GameFunction* gameFunction;

	XMFLOAT3 scale;
	XMFLOAT3 rotation;
	XMFLOAT3 bulletScale;
	XMFLOAT3 bulletRot;

	bool fireSwitch;
	bool moveSwitch;
	bool stopFlag;
	bool endMotion;
	bool moveMotion;	
	unsigned short countTime;

public:
	bool enterEnding;
	bool playerCollided;
	bool isPlayerBulletCollided[5];
	bool isCollision[5];
	XMFLOAT3 position[5];
	XMFLOAT3 bulletPos[5];
};

