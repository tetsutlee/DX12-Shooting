#include"Particle.h"

Particle::Particle(DirectX12* dx12, Input* input)
{
	scale = { 0.1f, 0.1f, 0.1f };
	rotation = { 0.0f, 0.0f, 0.0f };
	
	for (int i = 0; i < 15; i++)
	{
		position[i] = { (float)(rand() % 10 + 10), (float)(rand() % 5), 0.0f };
	}
	for (int j = 15; j < 30; j++)
	{
		position[j] = { (float)(rand() % 10 + 10), (float)-(rand() % 5), 0.0f };
	}
	for (int i = 0; i < 30; i++)
	{
		mesh[i] = nullptr;
		velocity[i] = { 0.0f, 0.0f, 0.0f };
	}
	this->dx12 = dx12;
	this->input = input;

	changeMotion = false;
	countTime = 0;
}

Particle::~Particle()
{
	
}

void Particle::Initialize(WNDCLASSEX& wndc, HWND& hwnd, const int width, const int height)
{
	srand((unsigned)time(NULL));

	for (int i = 0; i < 30; i++)
	{
		mesh[i] = new Mesh(dx12, input);
		mesh[i]->Initialize(wndc, hwnd, width, height, L"Resources/white.jpg");
		velocity[i] = { (float)(rand() % 4 * -0.02f), 0.0f , 0.0f };
	}
	for (int i = 0; i < 15; i++)
	{
		position[i] = { (float)(rand() % 10 + 10), (float)(rand() % 5), 0.0f };
	}
	for (int j = 15; j < 30; j++)
	{
		position[j] = { (float)(rand() % 10 + 10), (float)-(rand() % 5), 0.0f };
	}
}

void Particle::Update(WNDCLASSEX& wndc, HWND& hwnd, const int width, const int height)
{
	rotation.x += 2.0f;
	rotation.y += 2.0f;
	rotation.z += 2.0f;

	for (int i = 0; i < 15; i++)
	{
		if (position[i].x <= -10)
		{
			position[i] = { (float)(rand() % 10 + 10), (float)(rand() % 5), 0.0f };
		}
	}
	for (int j = 15; j < 30; j++)
	{
		if (position[j].x <= -10)
		{
			position[j] = { (float)(rand() % 10 + 10), (float)-(rand() % 5), 0.0f };
		}
	}

	for (int i = 0; i < 30; i++)
	{
		position[i].x -= 0.05f;
		position[i].x = position[i].x + velocity[i].x;

		position[i].y -= 0.004f;
		if (position[i].y <= 0.1f)
		{
			position[i].y += 0.004f;
		}

		mesh[i]->Update(scale, rotation, position[i], width, height);
	}
}

void Particle::Draw()
{
	for (int i = 0; i < 30; i++)
	{
		mesh[i]->Draw();
	}
}