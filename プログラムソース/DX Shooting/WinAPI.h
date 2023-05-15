#pragma once
#include<Windows.h>

class WinAPI final
{
private:
	WinAPI();
	~WinAPI();
	
public:
	static WinAPI* GetInstance();
	void Initialize();
	const int windowWidth = 1280;
	const int windowHeight = 720;
	WNDCLASSEX w;
	RECT wrc;
	HWND hwnd;
};
