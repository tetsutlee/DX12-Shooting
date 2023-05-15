#include"GameManager.h"

GameManager::GameManager(DirectX12* dx12, Input* input)
{
	endingScene = false;
	title = nullptr;
	ending = nullptr;
	background = nullptr;
	player = nullptr;
	enemy = nullptr;
	this->dx12 = dx12;
	this->input = input;
}

GameManager::~GameManager()
{
	delete player;
	delete enemy;
	delete title;
	delete ending;
	delete background;
}

void GameManager::Initialize(WNDCLASSEX& wndc, HWND& hwnd, const int width, const int height)
{
	endingScene = false;
	player = new Player(dx12, input);
	player->Initialize(wndc, hwnd, width, height);
	enemy = new Enemy(dx12, input, player);
	enemy->Initialize(wndc, hwnd, width, height);
	background = new Background(dx12, input);
	background->Initialize(wndc, hwnd, width, height);
	ending = new Ending(dx12,input);
	ending->Initialize(wndc, hwnd, width, height);
	title = new Title(dx12, input);
	title->Initialize(wndc, hwnd, width, height);
}

void GameManager::UpdateAndDraw(WNDCLASSEX& wndc, HWND& hwnd, const int width, const int height)
{
	background->Update(wndc, hwnd, width, height);
	background->Draw();
	if (title->GetStartFlag() && !enemy->enterEnding)
	{
		player->Update(width, height);
		if (!enemy->isCollision[0] && !enemy->isCollision[1] && !enemy->isCollision[2]
			&& !enemy->isCollision[3] && !enemy->isCollision[4] && !enemy->playerCollided)
		{
			player->Draw();
		}
		enemy->Update(width, height);
		enemy->Draw();
	}
	if (!title->GetStartFlag() && !enemy->enterEnding)
	{
		title->Update(wndc, hwnd, width, height);
		title->Draw();
	}
	if (enemy->enterEnding)
	{
		endingScene = true;
		background->Update(wndc, hwnd, width, height);
		ending->Update(wndc, hwnd, width, height);
		ending->Draw();
	}
	
}


