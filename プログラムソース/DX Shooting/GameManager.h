#pragma once
#include"Title.h"
#include"EndingScene.h"
#include"Background.h"
#include"Player.h"
#include"Enemy.h"

class GameManager
{
public:
	GameManager(DirectX12* dx12, Input* input);
	~GameManager();
	void Initialize(WNDCLASSEX& wndc, HWND& hwnd, const int width, const int height);
	void UpdateAndDraw(WNDCLASSEX& wndc, HWND& hwnd, const int width, const int height);

	void SetEndingScene(bool endingScene) { this->endingScene = endingScene; }
	bool GetEndingScene() { return endingScene; }

private:
	DirectX12* dx12;
	Input* input;
	Player* player;
	Enemy* enemy;
	Background* background;
	Title* title;
	Ending* ending;

	bool endingScene;
};

