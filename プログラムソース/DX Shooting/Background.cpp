#include"Background.h"

Background::Background(DirectX12* dx12, Input* input)
{
	this->dx12 = dx12;
	this->input = input;
	particle = new Particle(dx12, input);
}

Background::~Background()
{
	
}

void Background::Initialize(WNDCLASSEX& wndc, HWND& hwnd, const int width, const int height)
{
	particle->Initialize(wndc, hwnd, width, height);
}

void Background::Update(WNDCLASSEX& wndc, HWND& hwnd, const int width, const int height)
{
	particle->Update(wndc, hwnd, width, height);
}

void Background::Draw()
{
	particle->Draw();
}