#pragma once
#include"Input.h"
#include"DirectX12.h"
#include"Mesh.h"
#include<time.h>

class Particle
{
public:
	Particle(DirectX12* dx12, Input* input);
	~Particle();
	void Initialize(WNDCLASSEX& wndc, HWND& hwnd, const int width, const int height);
	void Update(WNDCLASSEX& wndc, HWND& hwnd, const int width, const int height);
	void Draw();

private:
	DirectX12* dx12;
	Input* input;
	Mesh* mesh[30];

	XMFLOAT3 scale;
	XMFLOAT3 rotation;
	XMFLOAT3 position[30];
	XMFLOAT3 velocity[30];
	bool changeMotion;
	unsigned short countTime;

};