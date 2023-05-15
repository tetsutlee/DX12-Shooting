#pragma once
#include"DirectX12.h"
#include<iostream>
#include<xaudio2.h>
#include<dsound.h>
#pragma comment(lib, "dsound.lib")//���C�u���������N
#pragma comment(lib, "dxguid.lib")//���C�u���������N

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
	LPDIRECTSOUNDBUFFER lpPrimary;       // �v���C�}���T�E���h�o�b�t�@
	LPDIRECTSOUNDBUFFER lpSecondary;     // �Z�J���_���T�E���h�o�b�t�@
};

