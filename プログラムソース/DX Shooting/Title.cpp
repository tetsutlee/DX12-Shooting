#include"Title.h"


Title::Title(DirectX12* dx12, Input* input)
{
	meshScale = { 1.0f, 1.0f, 1.0f };
	meshRotation = { 0.0f, -20.0f, 0.0f };
	meshPosition = { 0.0f, 0.0f, 0.0f };
	spriteScale = { 4.0f, 1.0f, 1.0f };
	spriteRotation = 0.0f;
	for (int i = 0; i < 2; i++)
	{
		sprite[i] = new Sprite(dx12, input);
	}
	this->dx12 = dx12;
	this->input = input;
	mesh = new Mesh(dx12, input);
	pressFlag = false;
	startFlag = false;
	animationFlag = false;
	countTime = 0;
	continueTime = 0;
}

Title::~Title()
{
}


void Title::Initialize(WNDCLASSEX& wndc, HWND& hwnd, const int width, const int height)
{
	mesh->Initialize(wndc, hwnd, width, height, L"Resources/texture.png");
	for (int i = 0; i < 2; i++)
	{
		sprite[i]->Generate(width, height);
	}
	sprite[0]->Initialize(wndc, hwnd, L"Resources/Enter.png");
	sprite[1]->Initialize(wndc, hwnd, L"Resources/DXShootingTitle.png");
}

void Title::Update(WNDCLASSEX& wndc, HWND& hwnd, const int width, const int height)
{
	mesh->Update(meshScale, meshRotation, meshPosition, width, height);
	mesh->Draw();
	sprite[0]->PipelineSetCommand(width, height, 0.0f, { 440.0f, 500.0f, 0.0f, 1.0f }, spriteScale);
	sprite[1]->PipelineSetCommand(width, height, 0.0f, { 300.0f, -20.0f, 0.0f, 1.0f }, { 7.0f, 4.0f, 5.0f });
}

void Title::Draw()
{
	if (input->KeyTrigger(DIK_RETURN))
	{
		pressFlag = true;
	}
	countTime++;
	if (!pressFlag)
	{
		meshRotation.z += 4.0f;
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
		meshRotation.z +=12.0f;
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