#pragma once
#define DIRECTINPUT_VERSION 0x0800 //DirectInput version
#include<dinput.h>
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

class Input
{
public:
	Input();
	~Input();
	HRESULT Initialize(WNDCLASSEX& wndc,HWND &hwnd);
	void Update();
	bool GetKey(int keyNumber);
	bool KeyTrigger(int keyNumber);
	bool KeyUp(int keyNumber);

public:
	BYTE currentKey[256] = {};
	BYTE previousKey[256] = {};
	IDirectInputDevice8* devkeyboard;
};
