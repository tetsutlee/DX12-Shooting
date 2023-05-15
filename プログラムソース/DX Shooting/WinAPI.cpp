#include"WinAPI.h"


WinAPI::WinAPI()
{
	w = {};
	wrc = {}; //Window Rectangle
	hwnd = {};
}

WinAPI::~WinAPI()
{
}

LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	//Seperate with message
	switch (msg) {
	case WM_DESTROY: //Closing the window
		PostQuitMessage(0); //Ending message for OS
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam); //Execute standard procedure
}

void WinAPI::Initialize()
{

	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProc; //Set window procedure
	w.lpszClassName = L"DX Shooting"; //Set window class name
	w.hInstance = GetModuleHandle(nullptr); //Set window handle
	w.hCursor = LoadCursor(NULL, IDC_ARROW); //Set window cusor

	//Register window class on OS
	RegisterClassEx(&w);
	//Window size{ X coordinate, Y coordinate, Width, Height }
	wrc = { 0, 0, windowWidth, windowHeight };
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false); //Automatic size adjustment

	//Generate window object
	hwnd = CreateWindow(w.lpszClassName, //Window class name
		L"DX Shooting", //Title bar string
		WS_OVERLAPPEDWINDOW, //Standard window style
		CW_USEDEFAULT, //Show X coordinate(use OS default)
		CW_USEDEFAULT, //Show Y coordinate(use OS default)
		wrc.right - wrc.left, //Window Width
		wrc.bottom - wrc.top, //Window Height
		nullptr, //Parent window handle
		nullptr, //Main window handle
		w.hInstance, //Call application handle
		nullptr); //Option

	//Show window
	ShowWindow(hwnd, SW_SHOW);
}

WinAPI* WinAPI::GetInstance() {
	static WinAPI instance;
	return &instance;
}