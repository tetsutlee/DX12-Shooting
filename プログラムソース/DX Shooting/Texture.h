#pragma once
#include"DirectX12.h"
#include<DirectXTex.h>
using namespace DirectX;

class Texture
{
public:
	Texture(DirectX12* dx12);
	void Load(std::wstring fileName);
	void Set();//���f���`�悷�鎞������int�^�̕ϐ�����������ASetGraphicsRootDescriptorTable�̃C���f�b�N�X���ς�邩��

public:
	DirectX12* dx12;

	ID3D12Resource* texbuff;
	ID3D12DescriptorHeap* textureDescHeap;//Generate shader resource view �V�F�[�_���\�[�X�r���[�̐���
};

