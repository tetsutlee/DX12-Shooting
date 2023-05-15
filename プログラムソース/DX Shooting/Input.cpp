#include"Input.h"

Input::Input() {
	devkeyboard = nullptr;
	for (int i = 0; i < 256; i++)
	{
		currentKey[i] = 0;
		previousKey[i] = 0;
	}

}

Input::~Input() {

}

HRESULT Input::Initialize(WNDCLASSEX& wndc, HWND& hwnd)
{
	//Generate DirectInput Object
	IDirectInput8* dinput = nullptr;
	auto result = DirectInput8Create(
		wndc.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);
	//Generate keyboard device
	result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);
	result = devkeyboard->SetDataFormat(&c_dfDIKeyboard); //Standard format
	//排他制御レベルのセット
	result = devkeyboard->SetCooperativeLevel(
		hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);

	memset(previousKey, 0, 256);
	memset(currentKey, 0, 256);

	return result;
}

void Input::Update() {
	memcpy(previousKey, currentKey, 256);
	auto result = devkeyboard->Acquire();
	result = devkeyboard->GetDeviceState(sizeof(currentKey), currentKey);
}

bool Input::GetKey(int keyNumber) {
	return currentKey[keyNumber];
}

bool Input::KeyTrigger(int keyNumber) {
	return currentKey[keyNumber] && !previousKey[keyNumber];
}

bool Input::KeyUp(int keyNumber) {
	return !currentKey[keyNumber] && previousKey[keyNumber];
}