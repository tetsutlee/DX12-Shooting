#pragma once
#include"DirectX12.h"
#include<iostream>
#include<xaudio2.h>
#include<dsound.h>
#pragma comment(lib, "dsound.lib")//ライブラリリンク
#pragma comment(lib, "dxguid.lib")//ライブラリリンク

class Sound
{
public:
	Sound();
	~Sound();
	void Initialize();
	void CleanUp();
	void UseSound();
	//bool Initialize(HWND& hwnd);
	//bool CreatePrimaryBuffer();
	//bool CreateSoundBuffer(LPDIRECTSOUNDBUFFER* dsb, const char* fileName);
	//bool Exit();

public:
	IXAudio2* xAudio;
	IXAudio2MasteringVoice* masteringVoice;
	IXAudio2SourceVoice* sourceVoice;

	LPDIRECTSOUND8 lpDS;     // DirectSound8
	LPDIRECTSOUNDBUFFER lpPrimary;       // プライマリサウンドバッファ
	LPDIRECTSOUNDBUFFER lpSecondary;     // セカンダリサウンドバッファ
};

