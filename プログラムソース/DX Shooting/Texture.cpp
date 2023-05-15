#include"Texture.h"

Texture::Texture(DirectX12* dx12)
{
	this->dx12 = dx12;
	texbuff = nullptr;
	textureDescHeap = nullptr;
}

void Texture::Load(std::wstring fileName)
{
	//WICテクスチャのロード
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	auto result = LoadFromWICFile(
		fileName.c_str(), //
		WIC_FLAGS_NONE,
		&metadata, scratchImg);

	const Image* img = scratchImg.GetImage(0, 0, 0); // 生データ抽出

#pragma region Texture Buffer Old code

	////テクスチャバッファの生成
    ////GPU側にテクスチャバッファを作成する
	//D3D12_HEAP_PROPERTIES texHeapProp{}; // テクスチャヒープ設定
	//texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	//texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	//texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	//D3D12_RESOURCE_DESC texresDesc{}; // リソース設定
	//texresDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(metadata.dimension);
	//texresDesc.Format = metadata.format;
	//texresDesc.Width = metadata.width; // 幅
	//texresDesc.Height = (UINT)metadata.height; // 高さ
	//texresDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	//texresDesc.MipLevels = (UINT16)metadata.mipLevels;
	//texresDesc.SampleDesc.Count = 1;

	//result = dx12->dev->CreateCommittedResource( // GPUリソースの生成
	//	&texHeapProp,
	//	D3D12_HEAP_FLAG_NONE,
	//	&texresDesc,
	//	D3D12_RESOURCE_STATE_GENERIC_READ, // テクスチャ用指定
	//	nullptr,
	//	IID_PPV_ARGS(&texbuff));

#pragma endregion

	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels
	);
	result = dx12->dev->CreateCommittedResource( // GPUリソースの生成
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, // テクスチャ用指定
		nullptr,
		IID_PPV_ARGS(&texbuff));

	result = texbuff->WriteToSubresource(
		0,
		nullptr, // 全領域へコピー
		img->pixels, // 元データアドレス
		(UINT)img->rowPitch, // 1ラインサイズ
		(UINT)img->slicePitch // 1枚サイズ
	);

	//DescriptorHeap for texture
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	descHeapDesc.NumDescriptors = 1;//SRV1つとCBV1つ
	result = dx12->dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&textureDescHeap));//生成

	//デスクリプタヒープの先頭ハンドルを取得しておく
	auto textureHeapHandle = textureDescHeap->GetCPUDescriptorHandleForHeapStart();

	// シェーダリソースビュー作成
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // 設定構造体
	//srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;//RGBA
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;
	dx12->dev->CreateShaderResourceView(texbuff, //ビューと関連付けるバッファ
		&srvDesc, //テクスチャ設定情報
		textureHeapHandle);

}

void Texture::Set()
{
	dx12->cmdList->SetDescriptorHeaps(1, &textureDescHeap);
	dx12->cmdList->SetGraphicsRootDescriptorTable(1, textureDescHeap->GetGPUDescriptorHandleForHeapStart());
}
