#pragma once
#include"DirectX12.h"
#include<DirectXMath.h>
using namespace DirectX;

class GameFunction
{
public:
	GameFunction();
	~GameFunction();
	bool BulletCollision(XMFLOAT3 playerBulletPos, XMFLOAT3 enemyPos);
	bool EnemyBulletCollision(XMFLOAT3 playerPos, XMFLOAT3 enemyBulletPos);
	bool BodyCollision(XMFLOAT3 playerPos, XMFLOAT3 enemyPos);

private:

};
