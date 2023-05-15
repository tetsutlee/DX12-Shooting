#pragma once
#include"Particle.h"

class Background
{
public:
	Background(DirectX12* dx12, Input* input);
	~Background();
	void Initialize(WNDCLASSEX& wndc, HWND& hwnd, const int width, const int height);
	void Update(WNDCLASSEX& wndc, HWND& hwnd, const int width, const int height);
	void Draw();

private:
	DirectX12* dx12;
	Input* input;
	Particle* particle;
};

