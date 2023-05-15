#include"Sprite.h"

struct SpriteConstBufferData 
{
	XMFLOAT4 couleur; //色(RGBA)
	XMMATRIX matrix; //3D変換行列
};

struct VertexPosUV
{
	XMFLOAT3 position; //xyz座標
	XMFLOAT2 uvPos; //uv座標
};

Sprite::Sprite(DirectX12* dx12, Input* input)
{
	vsBlob = nullptr; // Vertex Shader Object
	psBlob = nullptr; // Pixel Shader objecy
	errorBlob = nullptr; // Error object

	gpipeline = {};

	spriteRootsignature = nullptr;
	spritePipelineState = nullptr;
	spriteVertBuff = nullptr;
	spriteIndexBuff = nullptr;
	spriteConstBuff = nullptr;
	basicDescHeap = nullptr;
	basicHeapHandle = {};
	gpuDescHandleSRV = {};
	spriteVBView = {};

	spriteRotation = 45.0f;
	spritePosition = { 640.0f, 300.0f, 0.0f, 1.0f};
	spriteMatWorld = {};
	spriteMatProjection = {};

	texture = new Texture(dx12);
	this->dx12 = dx12;
	this->input = input;
	
}

Sprite::~Sprite()
{

}

HRESULT Sprite::Initialize(WNDCLASSEX& wndc, HWND& hwnd, std::wstring fileName)
{
	texture->Load(fileName);

	//Load and compile Vertex Shader
	auto result = D3DCompileFromFile(
		L"SpriteVertexShader.hlsl", // Shader file name
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // Set it includable
		"VSmain", "vs_5_0", // Entry poiint name, shader model
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // Setting for debug
		0,
		&vsBlob, &errorBlob);

	//Load and compile Pixel Shader
	result = D3DCompileFromFile(
		L"SpritePixelShader.hlsl", // Shader file name
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // Set it includable
		"PSmain", "ps_5_0", // Entry poiint name, shader model
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // Setting for debug
		0,
		&psBlob, &errorBlob);


	if (FAILED(result)) {
		//errorBlob からエラー内容を string 型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "¥n";
		//エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	//Vertex layout, in order to transfer vertex shader
	D3D12_INPUT_ELEMENT_DESC spriteInputLayout[] = {
		//XY Coordinate
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		//UV Coordinate(0614)
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
	};

	//Set vertex shader and pixel shader in pipeline
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//Set blend state
	gpipeline.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; // RBGA 全てのチャンネルを描画

	//Vertex layout
	gpipeline.InputLayout.pInputElementDescs = spriteInputLayout;
	gpipeline.InputLayout.NumElements = _countof(spriteInputLayout);

	//Set the shape to Triangle
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	//Other settings
	gpipeline.NumRenderTargets = 1; // 描画対象は 1 つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0~255 指定の RGBA
	gpipeline.SampleDesc.Count = 1; // 1 ピクセルにつき 1 回サンプリング
	//デプスステンシルステートの設定
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT; // 深度値フォーマット

	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; // 標準設定

	//共通設定
	blenddesc.BlendEnable = true; // ブレンドを有効にする
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD; // 加算
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE; // ソースの値を100% 使う
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO; // デストの値を 0% 使う

	//加算合成
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD; // 加算
	blenddesc.SrcBlend = D3D12_BLEND_ONE; // ソースの値を100% 使う
	blenddesc.DestBlend = D3D12_BLEND_ONE; // デストの値を100% 使う

	//ブレンドステートに設定する
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// GPUリソースの生成
	result = dx12->dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(SpriteConstBufferData) + 0xff) & ~0xff), //256バイトアラインメント
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&spriteConstBuff));

	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	descHeapDesc.NumDescriptors = 2;//SRV1つとCBV2つ
	result = dx12->dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&basicDescHeap));//生成

	//デスクリプタヒープの先頭ハンドルを取得しておく
	basicHeapHandle = basicDescHeap->GetCPUDescriptorHandleForHeapStart();

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
	cbvDesc.BufferLocation = spriteConstBuff->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = (UINT)spriteConstBuff->GetDesc().Width;
	//dx12->dev->CreateConstantBufferView(&cbvDesc, basicHeap->GetCPUDescriptorHandleForHeapStart());
	dx12->dev->CreateConstantBufferView(&cbvDesc, basicHeapHandle);
	// 消費した分だけアドレスをずらす
	basicHeapHandle.ptr += dx12->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	CD3DX12_ROOT_PARAMETER rootparams[2] = {};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	//Set Texture sampler(How to get color from the texture)
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	//Generate root signature
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);

	result = dx12->dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&spriteRootsignature));

	//パイプラインにルートシグネチャをセット
	gpipeline.pRootSignature = spriteRootsignature.Get();
	result = dx12->dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&spritePipelineState));
	

	return result;
}

HRESULT Sprite::Generate(const int width, const int height)
{
	HRESULT result = S_FALSE;

	VertexPosUV vertices[] = {
		// x y z u v
		{{ 0.0f, 100.0f, 0.0f}, {0.0f, 1.0f}}, // 左下
		{{ 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}, // 左上
		{{100.0f, 100.0f, 0.0f}, {1.0f, 1.0f}}, // 右下
		{{100.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}, // 右上
	};

	//Generate vertex buffer
	// GPU リソースの生成
	result = dx12->dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)), // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&spriteVertBuff));

	VertexPosUV* spriteVertMap = nullptr;
	result = spriteVertBuff->Map(0, nullptr, (void**)&spriteVertMap);
	memcpy(spriteVertMap, vertices, sizeof(vertices));//Copy the coordinates
	spriteVertBuff->Unmap(0, nullptr);	//Unmap vertex buffer

	//頂点バッファビューの作成
	spriteVBView.BufferLocation = spriteVertBuff->GetGPUVirtualAddress();
	spriteVBView.SizeInBytes = sizeof(vertices);
	spriteVBView.StrideInBytes = sizeof(vertices[0]);

	// GPUリソースの生成
	result = dx12->dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(spriteConstBuff) + 0xff) & ~0xff), //256バイトアラインメント
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&spriteConstBuff));

	return result;
}

void Sprite::PipelineSetCommand(const int width, const int height, float rotate, XMVECTOR position, XMFLOAT3 scale)
{
	spriteMatWorld = XMMatrixIdentity();
	spriteMatScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	spriteMatWorld *= spriteMatScale;
	spriteRotation = rotate;
	spriteMatWorld *= XMMatrixRotationZ(XMConvertToRadians(spriteRotation));
	spritePosition = position;
	spriteMatWorld *= XMMatrixTranslationFromVector(spritePosition);

	SpriteConstBufferData* spriteConstMap = nullptr;
	auto result = spriteConstBuff->Map(0, nullptr, (void**)&spriteConstMap); // マッピング
	spriteConstMap->couleur = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);
	spriteConstMap->matrix = XMMatrixOrthographicOffCenterLH(
		0.0f, (float)width, (float)height, 0.0f, 0.0f, 1.0f);
	spriteMatProjection = spriteConstMap->matrix = XMMatrixOrthographicOffCenterLH(
		0.0f, (float)width, (float)height, 0.0f, 0.0f, 1.0f);
	spriteConstMap->matrix = spriteMatWorld * spriteMatProjection;
	spriteConstBuff->Unmap(0, nullptr);

	dx12->cmdList->SetPipelineState(spritePipelineState.Get());
	dx12->cmdList->SetGraphicsRootSignature(spriteRootsignature.Get());
	dx12->cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

void Sprite::DrawCommand()
{
	//Descriptor heap array
	ID3D12DescriptorHeap* spritePpHeaps[] = { basicDescHeap.Get() };
	dx12->cmdList->SetDescriptorHeaps(_countof(spritePpHeaps), spritePpHeaps);

	D3D12_GPU_DESCRIPTOR_HANDLE spriteGpuDescHandle;
	// 定数バッファビュー(先頭)
	spriteGpuDescHandle = basicDescHeap->GetGPUDescriptorHandleForHeapStart();
	//dx12->cmdList->SetGraphicsRootDescriptorTable(0, gpuDescHandle);
	dx12->cmdList->SetGraphicsRootConstantBufferView(0, spriteConstBuff->GetGPUVirtualAddress());
	// シェーダリソースビュービュー(定数バッファの次)
	spriteGpuDescHandle.ptr += dx12->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	dx12->cmdList->SetGraphicsRootDescriptorTable(1, spriteGpuDescHandle);

	//Set vertex buffer view
	dx12->cmdList->IASetVertexBuffers(0, 1, &spriteVBView);

	texture->Set();

	dx12->cmdList->DrawInstanced(4, 1, 0, 0);

}