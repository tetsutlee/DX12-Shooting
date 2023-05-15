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
        //  Wave�t�H�[�}�b�g�̐ݒ�
        //
        WAVEFORMATEX format = { 0 };
        format.wFormatTag = WAVE_FORMAT_PCM;
        format.nChannels = 1;                   //�`�����l����
        format.wBitsPerSample = 16;             //�P�T���v��������̃r�b�g��
        format.nSamplesPerSec = 44100;          //�T���v�����O���g��
        format.nBlockAlign = format.wBitsPerSample / 8 * format.nChannels;
        format.nAvgBytesPerSec = format.nSamplesPerSec * format.nBlockAlign;

        //
        //  SourceVoice�̍쐬
        //
        if (FAILED(hr = xAudio->CreateSourceVoice(&sourceVoice, &format)))
            throw "CreateSourceVoice";


        //
        //  �K���ȉ����쐬
        //      �u���v��\��sin�g�i440Hz�A1�b�j
        //
        std::vector< BYTE > data(format.nAvgBytesPerSec * 1);     //�o�b�t�@
        short* p = (short*)&data[0];
        for (size_t i = 0; i < data.size() / 2; i++)
        {
            float length = format.nSamplesPerSec / 440.0f;          //�g��
            *p = (short)(32767 * sinf(i * 3.1415926535f / (length / 2)));
            p++;
        }

        //
        //  SourceVoice�Ƀf�[�^�𑗐M
        //
        XAUDIO2_BUFFER buffer = { 0 };
        buffer.AudioBytes = data.size();            //�o�b�t�@�̃o�C�g��
        buffer.pAudioData = &data[0];             //�o�b�t�@�̐擪�A�h���X
        buffer.Flags = XAUDIO2_END_OF_STREAM;       // tell the source voice not to expect any data after this buffer
        sourceVoice->SubmitSourceBuffer(&buffer);

        //
        //  �Đ�
        //
        sourceVoice->Start();

        MessageBox(0, L"�I�����܂�", L"", 0);

        //
        //  SourceVoice�̔j��
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