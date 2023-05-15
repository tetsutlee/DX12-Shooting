#pragma once
#include"Input.h"
#include"DirectX12.h"
#include"Sprite.h"
#include"Particle.h"

class Title
{
public:
	Title(DirectX12* dx12, Input* input);
	~Title();
	void Initialize(WNDCLASSEX& wndc, HWND& hwnd, const int width, const int height);
	void Update(WNDCLASSEX& wndc, HWND& hwnd, const int width, const int height);
	void Draw();

	void SetStartFlag(bool startFlag) { this->startFlag = startFlag; }
	bool GetStartFlag() { return startFlag; }
	
private:
	DirectX12* dx12;
	Input* input;
	Sprite* sprite[2];
	Mesh* mesh;

	XMFLOAT3 meshScale;
	XMFLOAT3 meshRotation;
	XMFLOAT3 meshPosition;
	XMFLOAT3 spriteScale;
	float spriteRotation;
	bool pressFlag;
	bool startFlag;
	bool animationFlag;
	unsigned short continueTime;
	unsigned short countTime;
};
