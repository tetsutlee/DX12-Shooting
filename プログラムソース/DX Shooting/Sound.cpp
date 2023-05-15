#include"Sound.h"

Sound::Sound()
{
    lpDS = NULL;
    lpPrimary = NULL;
    lpSecondary = NULL;
}

Sound::~Sound()
{
}

void Sound::Initialize()
{
    //
    //  Initialize XAudio2
    //
    HRESULT hr;
    if (FAILED(hr = CoInitializeEx(NULL, COINIT_MULTITHREADED)))
        throw "CoInitializeEx";

    UINT32 flags = 0;
#ifdef _DEBUG
    flags |= XAUDIO2_DEBUG_ENGINE;
#endif
    if (FAILED(hr = XAudio2Create(&xAudio, flags)))
        throw "XAudio2Create";

    //
    //  Create a mastering voice
    //
    if (FAILED(hr = xAudio->CreateMasteringVoice(&masteringVoice)))
        throw "CreateMasteringVoice";
}

void Sound::CleanUp()
{
    //
    // Cleanup XAudio2
    //
    if (masteringVoice != 0)
    {
        masteringVoice->DestroyVoice();
        masteringVoice = 0;
    }
    if (xAudio != 0)
    {
        xAudio->Release();
        xAudio = 0;
    }
    CoUninitialize();
}

void Sound::UseSound()
{
    try
    {
        this->Initialize();

        HRESULT hr;
        //
        //  Waveフォーマットの設定
        //
        WAVEFORMATEX format = { 0 };
        format.wFormatTag = WAVE_FORMAT_PCM;
        format.nChannels = 1;                   //チャンネル数
        format.wBitsPerSample = 16;             //１サンプルあたりのビット数
        format.nSamplesPerSec = 44100;          //サンプリング周波数
        format.nBlockAlign = format.wBitsPerSample / 8 * format.nChannels;
        format.nAvgBytesPerSec = format.nSamplesPerSec * format.nBlockAlign;

        //
        //  SourceVoiceの作成
        //
        if (FAILED(hr = xAudio->CreateSourceVoice(&sourceVoice, &format)))
            throw "CreateSourceVoice";


        //
        //  適当な音を作成
        //      「ラ」を表すsin波（440Hz、1秒）
        //
        std::vector< BYTE > data(format.nAvgBytesPerSec * 1);     //バッファ
        short* p = (short*)&data[0];
        for (size_t i = 0; i < data.size() / 2; i++)
        {
            float length = format.nSamplesPerSec / 440.0f;          //波長
            *p = (short)(32767 * sinf(i * 3.1415926535f / (length / 2)));
            p++;
        }

        //
        //  SourceVoiceにデータを送信
        //
        XAUDIO2_BUFFER buffer = { 0 };
        buffer.AudioBytes = data.size();            //バッファのバイト数
        buffer.pAudioData = &data[0];             //バッファの先頭アドレス
        buffer.Flags = XAUDIO2_END_OF_STREAM;       // tell the source voice not to expect any data after this buffer
        sourceVoice->SubmitSourceBuffer(&buffer);

        //
        //  再生
        //
        sourceVoice->Start();

        MessageBox(0, L"終了します", L"", 0);

        //
        //  SourceVoiceの破棄
        //
        sourceVoice->Stop();
        sourceVoice->DestroyVoice();
    }

    catch (const char* e)
    {
        std::cout << e;
    }

    this->CleanUp();
}