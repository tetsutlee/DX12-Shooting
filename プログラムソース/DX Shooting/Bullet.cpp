#include"Bullet.h"

Bullet::Bullet(DirectX12* dx12, Input* input)
{
	scale = { 0.3f, 0.3f, 0.8f };
	rotation = { 0.0f, -90.0f, -30.0f };
	position = { -9.0f, 0.0f, 0.0f };
	mesh = nullptr;
	this->dx12 = dx12;
	this->input = input;
}

Bullet::~Bullet()
{
	
}

void Bullet::Initialize(WNDCLASSEX& wndc, HWND& hwnd, const int width, const int height)
{
	mesh = new Mesh(dx12, input);
	mesh->Initialize(wndc, hwnd, width, height, L"Resources/effect.png");
}

void Bullet::Update(const int width, const int height)
{
	input->Update();

	rotation.z -= 7.0f;

	mesh->Update(scale, rotation, position, width, height);
}

void Bullet::Draw()
{
	mesh->Draw();
}