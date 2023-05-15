#pragma once
#include"DirectX12.h"
#include<DirectXTex.h>
using namespace DirectX;

class Texture
{
public:
	Texture(DirectX12* dx12);
	void Load(std::wstring fileName);
	void Set();//モデル描画する時引数にint型の変数を持たせる、SetGraphicsRootDescriptorTableのインデックスが変わるから

public:
	DirectX12* dx12;

	ID3D12Resource* texbuff;
	ID3D12DescriptorHeap* textureDescHeap;//Generate shader resource view シェーダリソースビューの生成
};

