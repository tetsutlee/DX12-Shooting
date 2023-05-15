#include"Player.h"

Player::Player(DirectX12* dx12, Input* input)
{
	scale = { 1.0f, 1.0f, 1.0f };
	rotation = { 0.0f, -90.0f, -30.0f };
	position = { -15.0f, 0.0f, 0.0f };
	bulletScale = { 0.3f, 0.3f, 0.8f };
	bulletRot = { 0.0f, -90.0f, -30.0f };
	bulletPos = { -9.0f, 0.0f, 0.0f };
	launchSwitch = false;
	startMotion = false;
	endMotion = false;
	for (int i = 0; i < 5; i++)
	{
		isCollision[i] = false;
	}
	time = 0;
	life = 100;
	for (int i = 0; i < 2; i++)
	{
		mesh[i] = nullptr;
		
	}
	this->dx12 = dx12;
	this->input = input;
}

Player::~Player()
{
	delete* mesh;
}

void Player::Initialize(WNDCLASSEX& wndc, HWND& hwnd, const int width, const int height)
{
	for (int i = 0; i < 2; i++)
	{
		mesh[i] = new Mesh(dx12, input);
	}
	mesh[0]->Initialize(wndc, hwnd, width, height, L"Resources/texture.png");
	mesh[1]->Initialize(wndc, hwnd, width, height, L"Resources/effect.png");
}

void Player::Update(const int width, const int height)
{
	bulletRot.z += 9.0f;
	rotation.z -= 2.0f;

	if (!endMotion)
	{
		if (!startMotion)
		{
			position.x += 0.1f;
			if (position.x >= -8.0f)
			{
				startMotion = true;
			}
		}
		else
		{
			position.x -= 0.05f;
			if (position.x <= -9.0f)
			{
				endMotion = true;
				position.x = -9.0f;
			}
		}
	}

	//Move
	if (input->GetKey(DIK_UP) || input->GetKey(DIK_DOWN) || input->GetKey(DIK_RIGHT) || input->GetKey(DIK_LEFT) && endMotion)
	{
		if (input->GetKey(DIK_UP)) { position.y += 0.15f; }
		else if (input->GetKey(DIK_DOWN)) { position.y -= 0.15f; }
		if (input->GetKey(DIK_RIGHT)) { position.x += 0.15f; }
		else if (input->GetKey(DIK_LEFT)) { position.x -= 0.15f; }
	}

	//Wall
	if (position.x <= -9.0f)
	{
		position.x = -9.0f;
	}
	else if (position.x >= 9.0f)
	{
		position.x = 9.0f;
	}
	if (position.y >= 4.5f)
	{
		position.y = 4.5f;
	}
	else if (position.y <= -4.5f)
	{
		position.y = -4.5f;
	}


	//Bullet launch 
	if (input->KeyTrigger(DIK_SPACE))
	{
		launchSwitch = true;
		OutputDebugStringA("Fire!\n");
	}
	if (launchSwitch)
	{
		time++;
		bulletPos.x += 0.4f;
		if (time >= 60)
		{
			launchSwitch = false;
		}
	}
	else
	{
		time = 0;
		bulletPos = position;
	}

	mesh[1]->Update(bulletScale, bulletRot, bulletPos, width, height);
	mesh[0]->Update(scale, rotation, position, width, height);
}

void Player::Draw()
{
	mesh[1]->Draw();
	mesh[0]->Draw();
}