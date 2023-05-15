#pragma once
#include"Input.h"
#include"DirectX12.h"
#include"Mesh.h"

class Bullet
{
public:
	Bullet(DirectX12* dx12, Input* input);
	~Bullet();
	void Initialize(WNDCLASSEX& wndc, HWND& hwnd, const int width, const int height);
	void Update(const int width, const int height);
	void Draw();

	//void SetPosition(XMFLOAT3 pos) { this->position = pos; }
	//XMFLOAT3 GetPosition() const { return position; }

private:
	DirectX12* dx12;
	Input* input;
	Mesh* mesh;

	XMFLOAT3 scale;
	XMFLOAT3 rotation;

public:
	XMFLOAT3 position;
};

