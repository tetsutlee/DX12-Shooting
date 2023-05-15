#pragma once
#include"Input.h"
#include"DirectX12.h"
#include"Sprite.h"

class Ending
{
public:
	Ending(DirectX12* dx12, Input* input);
	~Ending();
	void Initialize(WNDCLASSEX& wndc, HWND& hwnd, const int width, const int height);
	void Update(WNDCLASSEX& wndc, HWND& hwnd, const int width, const int height);
	void Draw();

private:
	DirectX12* dx12;
	Input* input;
	Sprite* sprite[2];

	bool pressFlag;
	bool startFlag;
	bool animationFlag;
	unsigned short continueTime;
	unsigned short countTime;
};

