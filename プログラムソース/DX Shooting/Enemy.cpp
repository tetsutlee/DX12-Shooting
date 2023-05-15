#include"Enemy.h"

Enemy::Enemy(DirectX12* dx12, Input* input, Player* player)
{
	scale = { 1.1f, 1.1f, 1.1f };
	rotation = { 0.0f, 90.0f, 30.0f };
	bulletScale = { 0.4f, 0.4f, 0.4 };
	bulletRot = { 0.0f, 90.0f, 30.0f };
	for (int i = 0; i < 3; i++)
	{
		position[i] = { (float)(rand() % 10 + 10), (float)(rand() % 5), 0.0f };
		bulletPos[i] = { 15.0f, 0.0f, 0.0f };
	}
	for (int j = 3; j < 5; j++)
	{
		position[j] = { (float)(rand() % 10 + 10), (float)-(rand() % 5), 0.0f };
		bulletPos[j] = { 15.0f, 0.0f, 0.0f };
	}
	for (int i = 0; i < 10; i++)
	{
		mesh[i] = nullptr;
	}
	for (int i = 0; i < 5; i++)
	{
		isCollision[i] = false;
		isPlayerBulletCollided[i] = false;
	}
	enterEnding = false;
	playerCollided = false;
	fireSwitch = true;
	moveSwitch = false;
	stopFlag = false;
	endMotion = false;
	moveMotion = true;
	countTime = 0;
	this->dx12 = dx12;
	this->input = input;
	this->player = player;
	gameFunction = nullptr;
}

Enemy::~Enemy()
{
	delete* mesh;
}

void Enemy::Initialize(WNDCLASSEX& wndc, HWND& hwnd, const int width, const int height)
{
	for (int i = 0; i < 10; i++)
	{
		mesh[i] = new Mesh(dx12, input);
	}
	for (int i = 0; i < 5; i++)
	{
		mesh[i]->Initialize(wndc, hwnd, width, height, L"Resources/enemyEffect.jpg");
	}
	for (int j = 5; j < 10; j++)
	{
		mesh[j]->Initialize(wndc, hwnd, width, height, L"Resources/enemyBulletEffect.jpg");
	}
	gameFunction = new GameFunction();
}

void Enemy::Update(const int width, const int height)
{
	srand((unsigned)time(NULL));

	bulletRot.z -= 7.0f;
	rotation.z += 5.0f;

	for (int i = 0; i < 5; i++)
	{
		if (!stopFlag)
		{
			if (!moveSwitch)
			{
				position[i].x -= 0.12f;
			}
			if (position[i].x <= 8.0)
			{
				moveSwitch = true;
			}
			if (moveSwitch)
			{
				position[i].x += 0.05f;
				if (position[i].x >= 9.0f)
				{
					position[i].x = 9.0f;
					stopFlag = true;
				}
			}
		}
	}
	
	

	for (int i = 0; i < 5; i++)
	{
		position[i].x -= 0.12f;
		//Wall
		if (position[i].y >= 4.5f)
		{
			position[i].y = 4.5f;
		}
		else if (position[i].y <= -4.5f)
		{
			position[i].y = -4.5f;
		}
		
		if (stopFlag)
		{
			countTime++;
			if (countTime % 100 == 0)
			{
				fireSwitch = true;
			}
			if (countTime % 600 == 0)
			{
				fireSwitch = false;
			}
			if (fireSwitch)
			{
				bulletPos[0].x -= 0.1f;
			}
			else
			{
				bulletPos[0] = position[0];
			}
		}
	}

	for (int i = 0; i < 3; i++)
	{
		if (position[i].x <= -10.0f)
		{
			isCollision[i] = false;
			isPlayerBulletCollided[i] = false;
			playerCollided = false;
			position[i] = { (float)(rand() % 10 + 10), (float)(rand() % 5), 0.0f };
		}
	}
	for (int j = 3; j < 5; j++)
	{
		if (position[j].x <= -10.0f)
		{
			isCollision[j] = false;
			isPlayerBulletCollided[j] = false;
			playerCollided = false;
			position[j] = { (float)(rand() % 10 + 10), (float)-(rand() % 5), 0.0f };
		}
	}

	//Body Collision and Player Bullet Collision
	if (gameFunction->BodyCollision(player->position, position[0]) && 
		!gameFunction->BulletCollision(player->bulletPos, position[0]) &&
		!gameFunction->BulletCollision(player->bulletPos, position[1]) &&
		!gameFunction->BulletCollision(player->bulletPos, position[2]) &&
		!gameFunction->BulletCollision(player->bulletPos, position[3]) &&
		!gameFunction->BulletCollision(player->bulletPos, position[4]))
	{
		player->life--;
		isCollision[0] = true;
	}
	if (gameFunction->BodyCollision(player->position, position[1]) &&
		!gameFunction->BulletCollision(player->bulletPos, position[0]) &&
		!gameFunction->BulletCollision(player->bulletPos, position[1]) &&
		!gameFunction->BulletCollision(player->bulletPos, position[2]) &&
		!gameFunction->BulletCollision(player->bulletPos, position[3]) &&
		!gameFunction->BulletCollision(player->bulletPos, position[4]))
	{
		player->life--;
		isCollision[1] = true;
	}
	if (gameFunction->BodyCollision(player->position, position[2]) &&
		!gameFunction->BulletCollision(player->bulletPos, position[0]) &&
		!gameFunction->BulletCollision(player->bulletPos, position[1]) &&
		!gameFunction->BulletCollision(player->bulletPos, position[2]) &&
		!gameFunction->BulletCollision(player->bulletPos, position[3]) &&
		!gameFunction->BulletCollision(player->bulletPos, position[4]))
	{
		player->life--;
		isCollision[2] = true;
	}
	if (gameFunction->BodyCollision(player->position, position[3]) &&
		!gameFunction->BulletCollision(player->bulletPos, position[0]) &&
		!gameFunction->BulletCollision(player->bulletPos, position[1]) &&
		!gameFunction->BulletCollision(player->bulletPos, position[2]) &&
		!gameFunction->BulletCollision(player->bulletPos, position[3]) &&
		!gameFunction->BulletCollision(player->bulletPos, position[4]))
	{
		player->life--;
		isCollision[3] = true;
	}
	if (gameFunction->BodyCollision(player->position, position[4]) &&
		!gameFunction->BulletCollision(player->bulletPos, position[0]) &&
		!gameFunction->BulletCollision(player->bulletPos, position[1]) &&
		!gameFunction->BulletCollision(player->bulletPos, position[2]) &&
		!gameFunction->BulletCollision(player->bulletPos, position[3]) &&
		!gameFunction->BulletCollision(player->bulletPos, position[4]))
	{
		player->life--;
		isCollision[4] = true;
	}
	if (gameFunction->EnemyBulletCollision(player->position, bulletPos[0]) &&
		!gameFunction->BulletCollision(player->bulletPos, position[0]) &&
		!gameFunction->BulletCollision(player->bulletPos, position[1]) &&
		!gameFunction->BulletCollision(player->bulletPos, position[2]) &&
		!gameFunction->BulletCollision(player->bulletPos, position[3]) &&
		!gameFunction->BulletCollision(player->bulletPos, position[4]) &&
		!gameFunction->BodyCollision(player->position, position[0]) &&
		!gameFunction->BodyCollision(player->position, position[1]) &&
		!gameFunction->BodyCollision(player->position, position[2]) &&
		!gameFunction->BodyCollision(player->position, position[3]) &&
		!gameFunction->BodyCollision(player->position, position[4]))
	{
		player->life--;
		playerCollided = true;
	}


	//Player Bullet Collided
	if (gameFunction->BulletCollision(player->bulletPos, position[0]) &&
		!gameFunction->BodyCollision(player->position, position[0]) &&
		!gameFunction->BodyCollision(player->position, position[1]) &&
		!gameFunction->BodyCollision(player->position, position[2]) &&
		!gameFunction->BodyCollision(player->position, position[3]) &&
		!gameFunction->BodyCollision(player->position, position[4]))
	{
		isPlayerBulletCollided[0] = true;
	}
	if (gameFunction->BulletCollision(player->bulletPos, position[1]) &&
		!gameFunction->BodyCollision(player->position, position[0]) &&
		!gameFunction->BodyCollision(player->position, position[1]) &&
		!gameFunction->BodyCollision(player->position, position[2]) &&
		!gameFunction->BodyCollision(player->position, position[3]) &&
		!gameFunction->BodyCollision(player->position, position[4]))
	{
		isPlayerBulletCollided[1] = true;
	}
	if (gameFunction->BulletCollision(player->bulletPos, position[2]) &&
		!gameFunction->BodyCollision(player->position, position[0]) &&
		!gameFunction->BodyCollision(player->position, position[1]) &&
		!gameFunction->BodyCollision(player->position, position[2]) &&
		!gameFunction->BodyCollision(player->position, position[3]) &&
		!gameFunction->BodyCollision(player->position, position[4]))
	{
		isPlayerBulletCollided[2] = true;
	}
	if (gameFunction->BulletCollision(player->bulletPos, position[3]) &&
		!gameFunction->BodyCollision(player->position, position[0]) &&
		!gameFunction->BodyCollision(player->position, position[1]) &&
		!gameFunction->BodyCollision(player->position, position[2]) &&
		!gameFunction->BodyCollision(player->position, position[3]) &&
		!gameFunction->BodyCollision(player->position, position[4]))
	{
		isPlayerBulletCollided[3] = true;
	}
	if (gameFunction->BulletCollision(player->bulletPos, position[4]) &&
		!gameFunction->BodyCollision(player->position, position[0]) &&
		!gameFunction->BodyCollision(player->position, position[1]) &&
		!gameFunction->BodyCollision(player->position, position[2]) &&
		!gameFunction->BodyCollision(player->position, position[3]) &&
		!gameFunction->BodyCollision(player->position, position[4]))
	{
		isPlayerBulletCollided[4] = true;
	}

	//Entering ending scene
	if (player->life == 0)
	{
		enterEnding = true;
	}

	mesh[5]->Update(bulletScale, bulletRot, bulletPos[0], width, height);
	
	for (int i = 0; i < 5; i++)
	{
		mesh[i]->Update(scale, rotation, position[i], width, height);
	}
}

void Enemy::Draw()
{
	if (!isCollision[0] && !isPlayerBulletCollided[0])
	{
		mesh[5]->Draw();
	}

	if (!isCollision[1] && !isPlayerBulletCollided[1])
	{
		mesh[1]->Draw();
	}
	if (!isCollision[2] && !isPlayerBulletCollided[2])
	{
		mesh[2]->Draw();
	}
	if (!isCollision[3] && !isPlayerBulletCollided[3])
	{
		mesh[3]->Draw();
	}
	if (!isCollision[4] && !isPlayerBulletCollided[4])
	{
		mesh[4]->Draw();
	}

	if (!isCollision[0] && !isPlayerBulletCollided[0])
	{
		mesh[0]->Draw();
	}

}