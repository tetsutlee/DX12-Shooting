#include"EndingScene.h"

Ending::Ending(DirectX12* dx12, Input* input)
{
	pressFlag = 0;
	startFlag = 0;
	animationFlag = 0;
	continueTime = 0;
	countTime = 0;
	for (int i = 0; i < 2; i++)
	{
		sprite[i] = nullptr;
	}
	this->dx12 = dx12;
	this->input = input;
}

Ending::~Ending()
{
}

void Ending::Initialize(WNDCLASSEX& wndc, HWND& hwnd, const int width, const int height)
{
	for (int i = 0; i < 2; i++)
	{
		sprite[i] = new Sprite(dx12, input);
		sprite[i]->Generate(width, height);
	}
	sprite[0]->Initialize(wndc, hwnd, L"Resources/Enter.png");
	sprite[1]->Initialize(wndc, hwnd, L"Resources/Ending.png");
}

void Ending::Update(WNDCLASSEX& wndc, HWND& hwnd, const int width, const int height)
{
	sprite[0]->PipelineSetCommand(width, height, 0.0f, { 440.0f, 500.0f, 0.0f, 1.0f }, { 4.0f, 1.0f, 1.0f });
	sprite[1]->PipelineSetCommand(width, height, 0.0f, { 300.0f, -20.0f, 0.0f, 1.0f }, { 7.0f, 5.0f, 5.0f });
}

void Ending::Draw()
{
	if (input->KeyTrigger(DIK_RETURN))
	{
		pressFlag = true;
	}
	countTime++;
	if (!pressFlag)
	{
		if (countTime % 40 == 0)
		{
			animationFlag = true;
		}
		if (countTime % 80 == 0)
		{
			animationFlag = false;
		}
	}
	else
	{
		if (countTime % 10 == 0)
		{
			animationFlag = true;
		}
		if (countTime % 20 == 0)
		{
			animationFlag = false;
		}
		continueTime++;
		if (continueTime >= 120)
		{
			startFlag = true;
		}
	}

	if (!animationFlag)
	{
		sprite[0]->DrawCommand();
	}
	sprite[1]->DrawCommand();

}
