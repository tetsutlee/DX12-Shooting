#include"WinAPI.h"
#include"Input.h"
#include"DirectX12.h"
#include"GameManager.h"
//#include"Sound.h"

//Entry point of Windows application(Main Function)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_  HINSTANCE, _In_ LPSTR, _In_ int) {

#pragma region Window Initialize
	WinAPI* winAPI = WinAPI::GetInstance();
	winAPI->Initialize();
#pragma endregion

#pragma region Input Initialize

	Input* input = new Input;
	input->Initialize(winAPI->w, winAPI->hwnd);

#pragma endregion

#pragma region DirectX Initialize
	//std::shared_ptr<DirectX12>;

	DirectX12* dx12 = new DirectX12;
	dx12->Initialize(winAPI->w, winAPI->hwnd);

	GameManager* gameManager = new GameManager(dx12, input);
	gameManager->Initialize(winAPI->w, winAPI->hwnd, winAPI->windowWidth, winAPI->windowHeight);

#pragma endregion

#pragma region Other Value
	float clearColor[] = { 0.0f, 0.0f, 0.2f, 0.0f }; // 青っぽい色	
#pragma endregion

	MSG msg{}; //message

	while (true)
	{
		//if there is message
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg); //Keyboard message process
			DispatchMessage(&msg); //Send message to WinProcedure
		}
		//if quit message showed, end the loop
		if (msg.message == WM_QUIT) {
			break;
		}


//////////////DirectX Update Start

#pragma region 1.リソースバリアを変更
		input->Update();
#pragma endregion		

#pragma region 2.画面クリアコマンドここから
		// 2.画面クリアコマンドここから
		dx12->UpdateStart(clearColor);		
		// 2.画面クリアコマンドここまで
#pragma endregion

#pragma region 3.描画コマンドここから	
		
		// 3.描画コマンドここから
		
		gameManager->UpdateAndDraw(winAPI->w, winAPI->hwnd, winAPI->windowWidth, winAPI->windowHeight);
		if (gameManager->GetEndingScene())
		{
			if (input->KeyTrigger(DIK_RETURN))
			{
				gameManager->Initialize(winAPI->w, winAPI->hwnd, winAPI->windowWidth, winAPI->windowHeight);
			}
		}
		
		// 3.描画コマンドここまで		

#pragma endregion


#pragma region 4.リソースバリアを戻す

		dx12->UpdateEnd();

#pragma endregion

//////////////DirectX Update End

	}
	delete input;
	delete dx12;
	delete gameManager;
	//Unregister window class
	UnregisterClass(winAPI->w.lpszClassName, winAPI->w.hInstance);

	return 0;
}