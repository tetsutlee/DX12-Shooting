#include"GameFunction.h"

GameFunction::GameFunction()
{
}

GameFunction::~GameFunction()
{
}

bool GameFunction::BulletCollision(XMFLOAT3 playerBulletPos, XMFLOAT3 enemyPos)
{
	if (playerBulletPos.x + 1.0f >= enemyPos.x &&
		playerBulletPos.x <= enemyPos.x + 1.4f &&
		playerBulletPos.y + 1.2f >= enemyPos.y &&
		playerBulletPos.y <= enemyPos.y + 1.4f)
	{
		OutputDebugStringA("Collided!!!");
		return true;
	}
	else
	{
		return false;
	}
}

bool GameFunction::EnemyBulletCollision(XMFLOAT3 playerPos, XMFLOAT3 enemyBulletPos)
{
	if (enemyBulletPos.x - 1.0f <= playerPos.x &&
		enemyBulletPos.x >= playerPos.x - 1.4f &&
		enemyBulletPos.y - 1.2f <= playerPos.y &&
		enemyBulletPos.y >= playerPos.y - 1.4f)
	{
		OutputDebugStringA("Collided!!!");
		return true;
	}
	else
	{
		return false;
	}
}

bool GameFunction::BodyCollision(XMFLOAT3 playerPos, XMFLOAT3 enemyPos)
{
	if (playerPos.x + 2.2f >= enemyPos.x &&
		playerPos.x <= enemyPos.x + 2.2f &&
		playerPos.y + 1.5f >= enemyPos.y &&
		playerPos.y <= enemyPos.y + 1.7f)
	{
		OutputDebugStringA("Collided!!!");
		return true;
	}
	else
	{
		return false;
	}
}

