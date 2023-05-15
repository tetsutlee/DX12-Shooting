#include"Mesh.h"

//XMFLOAT3 circleVertices[31];
//unsigned short circleIndices[90];

//In order to send the color data to pixel shader
// 定数バッファ用データ構造体

struct ConstBufferData {
	XMFLOAT4 color; // 色 (RGBA)
	XMMATRIX mat; // ３Ｄ変換行列
};

struct Vertex
{
	XMFLOAT3 pos; // xyz座標
	XMFLOAT3 normal; // 法線ベクトル
	XMFLOAT2 uv; // uv座標
};

#pragma region 立方体頂点データ

//Vertex vertices[] = {
//	// x y z n u v
//
//	//Front
//	{{-50.0f, -50.0f, -50.0f}, {}, {0.0f, 1.0f}}, // 左下
//	{{-50.0f, 50.0f, -50.0f}, {},{0.0f, 0.0f}}, // 左上	
//	{{ 50.0f, -50.0f, -50.0f}, {}, {1.0f, 1.0f}}, // 右下		
//	{{ 50.0f, 50.0f, -50.0f}, {},{1.0f, 0.0f}}, // 右上
//	//Back
//	{{-50.0f, -50.0f, 50.0f}, {},{0.0f, 1.0f}}, // 左下
//	{{ 50.0f, -50.0f, 50.0f}, {},{1.0f, 1.0f}}, // 右下	
//	{{-50.0f, 50.0f, 50.0f}, {},{0.0f, 0.0f}}, // 左上		
//	{{ 50.0f, 50.0f, 50.0f}, {},{1.0f, 0.0f}}, // 右上
//	//Left
//	{{-50.0f, -50.0f, -50.0f}, {},{0.0f, 1.0f}}, // 左下
//	{{-50.0f, -50.0f, 50.0f}, {},{0.0f, 0.0f}}, // 左上
//	{{-50.0f, 50.0f, -50.0f}, {},{1.0f, 1.0f}}, // 右下	
//	{{-50.0f, 50.0f, 50.0f}, {},{1.0f, 0.0f}}, // 右上
//	//Right
//	{{50.0f, -50.0f,-50.0f}, {},{0.0f, 1.0f}}, // 左下	
//	{{50.0f, 50.0f,-50.0f}, {},{1.0f, 1.0f}}, // 右下
//	{{50.0f, -50.0f, 50.0f}, {},{0.0f, 0.0f}}, // 左上
//	{{50.0f, 50.0f, 50.0f}, {},{1.0f, 0.0f}}, // 右上
//	//Up
//	{{-50.0f, -50.0f, -50.0f}, {},{0.0f, 1.0f}}, // 左下
//	{{ 50.0f, -50.0f, -50.0f}, {},{1.0f, 1.0f}}, // 右下	
//	{{-50.0f, -50.0f, 50.0f}, {},{0.0f, 0.0f}}, // 左上
//	{{ 50.0f, -50.0f, 50.0f}, {},{1.0f, 0.0f}}, // 右上
//	//Down
//	{{-50.0f, 50.0f, -50.0f}, {},{0.0f, 1.0f}}, // 左下
//	{{-50.0f, 50.0f, 50.0f}, {},{0.0f, 0.0f}}, // 左上	
//	{{ 50.0f, 50.0f, -50.0f}, {},{1.0f, 1.0f}}, // 右下
//	{{ 50.0f, 50.0f, 50.0f}, {},{1.0f, 0.0f}}, // 右上
//};

#pragma endregion 

#pragma region 立方体indices

//unsigned short indices[] = {
//   0, 1, 2, // 三角形1つ目
//   2, 1, 3, // 三角形2つ目
//   ///
//   4, 5, 6,
//   6, 5, 7,
//   ///
//   8, 9 , 10,
//   10, 9, 11,
//   ///
//   12, 13, 14,
//   14, 13, 15,
//   ///
//   16, 17, 18,
//   18, 17, 19,
//   ///
//   20, 21, 22,
//   22, 21, 23,
//
//};

#pragma endregion

Vertex vertices[5];
unsigned short indices[] = {
	1, 0, 3,
	2, 1, 3,
	0, 2, 3,
	0, 1, 4,
	1, 2, 4,
	2, 0, 4,
};

Mesh::Mesh(DirectX12* dx12, Input* input) {
	vertBuff = nullptr;// GPU リソースの生成
	indexBuff = nullptr;//Generate indices buffer 
	constBuff = nullptr;// GPUリソースの生成

	vbView = {};// 頂点バッファビューの作成
	ibView = {};

	vsBlob = nullptr; // Vertex Shader Object
	psBlob = nullptr; // Pixel Shader objecy
	errorBlob = nullptr; // Error object

	//basicHeap = nullptr;
	basicDescHeap = nullptr;
	basicHeapHandle = {};

	dsvHeap = nullptr;

	gpipeline = {};//Set graphics pipeline(struct)
	rootsignature = nullptr;
	pipelinestate = nullptr;
	pipelinestateWire = nullptr;

	scissorrect = {};
	viewport = {};
	controlSwitch = false;

	number = 3;
	radius = 0.25f;
	PI = 3.1415926535f;

	matProjection = {};
	matView = {};
	matWorld = {};

	matScale = {};
	matTrans = {};
	matRot = {};

	this->dx12 = dx12;
	this->input = input;
	camera = new Camera(input);
	texture = new Texture(dx12);
}

HRESULT Mesh::Initialize(WNDCLASSEX& wndc, HWND& hwnd, const int width, const int height, std::wstring fileName) {

	texture->Load(fileName);

	vertices[0].pos.x = 1;
	vertices[0].pos.y = 0;
	vertices[0].pos.z = 0;
	vertices[0].uv = { 0, 0 };
	vertices[1].pos.x = cosf(4.0f / number * PI);
	vertices[1].pos.y = sinf(4.0f / number * PI);
	vertices[1].pos.z = 0;	
	vertices[1].uv = { 1, 0 };
	vertices[2].pos.x = cosf(2.0f / number * PI);
	vertices[2].pos.y = sinf(2.0f / number * PI);
	vertices[2].pos.z = 0;
	vertices[2].uv = { 0, 1};
	vertices[3].pos = { 0, 0 ,0 };
	vertices[3].uv = { 0.5, 0.5f };
	vertices[4].pos = { 0, 0 ,-topHeight };
	vertices[4].uv = { 1, 1 };

	for (int i = 0; i < 5; i++)
	{
		vertices[i].normal = {};
	}

	CD3DX12_RESOURCE_DESC depthResDesc{};
	depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_D32_FLOAT,
		width,
		height,
		1, 0,
		1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
	);

	// リソース生成
	ID3D12Resource* depthBuffer = nullptr;
	//ComPtr<ID3D12Resource> depthBuffer;
	auto result = dx12->dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE, // 深度値書き込みに使用
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		IID_PPV_ARGS(&depthBuffer));

	// 深度ビュー用デスクリプタヒープ作成
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1; // 深度ビューは1つ
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV; // デプスステンシルビュー

	result = dx12->dev->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));
	//result = dx12->dev->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap2));
	// 深度ビュー作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT; // 深度値フォーマット
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dx12->dev->CreateDepthStencilView(
		depthBuffer,
		&dsvDesc,
		dsvHeap->GetCPUDescriptorHandleForHeapStart());


	//Generate vertex buffer
	// GPU リソースの生成
	result = dx12->dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)), // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));

	// GPUリソースの生成
	result = dx12->dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(indices)), // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));

	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(vertices);
	vbView.StrideInBytes = sizeof(vertices[0]);

	//Set Index Buffer Command
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeof(indices);

	// Load and compile Vertex Shader
	result = D3DCompileFromFile(
		L"BasicVertexShader.hlsl", // Shader file name
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // Set it includable
		"VSmain", "vs_5_0", // Entry poiint name, shader model
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // Setting for debug
		0,
		&vsBlob, &errorBlob);

	// Load and compile Pixel Shader
	result = D3DCompileFromFile(
		L"BasicPixelShader.hlsl", // Shader file name
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // Set it includable
		"PSmain", "ps_5_0", // Entry poiint name, shader model
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // Setting for debug
		0,
		&psBlob, &errorBlob);


	if (FAILED(result)) {
		// errorBlob からエラー内容を string 型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "¥n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	//Vertex layout, in order to transfer vertex shader
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		//XY Coordinate
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		//法線ベクトル
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		//UV Coordinate(0614)
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
	};

	//Set vertex shader and pixel shader in pipeline
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//Set blend state
	gpipeline.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; // RBGA 全てのチャンネルを描画

	//Vertex layout
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	//Set the shape to Triangle
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	//Other settings
	gpipeline.NumRenderTargets = 1; // 描画対象は 1 つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0~255 指定の RGBA
	gpipeline.SampleDesc.Count = 1; // 1 ピクセルにつき 1 回サンプリング
	//デプスステンシルステートの設定
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT; // 深度値フォーマット

#pragma region Blend Setting

	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; // 標準設定

	//共通設定
	blenddesc.BlendEnable = true; // ブレンドを有効にする
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD; // 加算
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE; // ソースの値を100% 使う
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO; // デストの値を 0% 使う

	////加算合成
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD; // 加算
	//blenddesc.SrcBlend = D3D12_BLEND_ONE; // ソースの値を100% 使う
	//blenddesc.DestBlend = D3D12_BLEND_ONE; // デストの値を100% 使う
	////減算合成
	//blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT; // デストからソースを減算
	//blenddesc.SrcBlend = D3D12_BLEND_ONE; // ソースの値を100% 使う
	//blenddesc.DestBlend = D3D12_BLEND_ONE; // デストの値を100% 使う
	////色反転
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD; // 加算
	//blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR; // 1.0f-デストカラーの値
	//blenddesc.DestBlend = D3D12_BLEND_ZERO; // 使わない

	//半透明合成
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD; // 加算
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA; // ソースのアルファ値
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA; // 1.0f-ソースのアルファ値

	// ブレンドステートに設定する
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

#pragma endregion

	//////DescriptorHeap for texture
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	descHeapDesc.NumDescriptors = 2;//SRV1つとCBV2つ
	result = dx12->dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&basicDescHeap));//生成

	//デスクリプタヒープの先頭ハンドルを取得しておく
	basicHeapHandle = basicDescHeap->GetCPUDescriptorHandleForHeapStart();

	// GPUリソースの生成
	result = dx12->dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff), //256バイトアラインメント
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));


	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
	cbvDesc.BufferLocation = constBuff->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = (UINT)constBuff->GetDesc().Width;
	//dx12->dev->CreateConstantBufferView(&cbvDesc, basicHeap->GetCPUDescriptorHandleForHeapStart());
	dx12->dev->CreateConstantBufferView(&cbvDesc, basicHeapHandle);
	// 消費した分だけアドレスをずらす
	basicHeapHandle.ptr += dx12->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);


	CD3DX12_DESCRIPTOR_RANGE descRangeCBV, descRangeSRV;
	descRangeCBV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	CD3DX12_ROOT_PARAMETER rootparams[2] = {};
	rootparams[0].InitAsDescriptorTable(1, &descRangeCBV, D3D12_SHADER_VISIBILITY_ALL);
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
		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));
	//rootSigBlob->Release();
	//パイプラインにルートシグネチャをセット
	gpipeline.pRootSignature = rootsignature.Get();
	result = dx12->dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate));
	gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME; // ポリゴン内塗りつぶし
	result = dx12->dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestateWire));


	return result;
}

void Mesh::Update(XMFLOAT3 scale, XMFLOAT3 rotation, XMFLOAT3 position, const int width, const int height) {
	//Do not put Input Update

	// 3.描画コマンドここから

	//定数バッファにデータを転送する(To change color) 	
	ConstBufferData* constMap = nullptr;
	auto result = constBuff->Map(0, nullptr, (void**)&constMap); // マッピング
	constMap->color = XMFLOAT4(1.0f, 1.0f, 0.5f, 0.5f); // RGBAで半透明の赤
	constMap->mat = XMMatrixIdentity(); // 単位行列
	constMap->mat.r[0].m128_f32[0] = 2.0f / width;
	constMap->mat.r[1].m128_f32[1] = -2.0f / height;
	constMap->mat.r[3].m128_f32[0] = -1.0f;
	constMap->mat.r[3].m128_f32[1] = 1.0f;
	//透視投影(0701 Update)
	constMap->mat = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f), // 画角60度
		(float)width / height, // アスペクト比（画面横幅/画面縦幅）
		0.1f, 1000.0f // 前端、奥端
	);
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		(float)width / height,
		0.1f, 1000.0f
	);
	//Matrix View ビュー変換
	//matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
	camera->Update();
	//Matrix World ワールド変換
	matWorld = XMMatrixIdentity(); //単位行列を代入;
	//Matrix Rotation
	matRot = XMMatrixIdentity();

	matWorld = XMMatrixIdentity(); // ワールド行列は毎フレームリセット
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matWorld *= matScale;
	matRot = XMMatrixRotationZ(XMConvertToRadians(rotation.z)); // Z軸まわりに45度回転
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x)); // X軸まわりに45度回転
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	matWorld *= matRot;
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);
	matWorld *= matTrans;
	constMap->mat = matWorld * camera->matView * matProjection;
	constBuff->Unmap(0, nullptr); // マッピング解除

#pragma region Input Proccess



#pragma endregion

	for (int i = 0; i < 18 / 3; i++)
	{// 三角形１つごとに計算していく
	// 三角形のインデックスを取得
		unsigned short indices0 = indices[i * 3 + 0];
		unsigned short indices1 = indices[i * 3 + 1];
		unsigned short indices2 = indices[i * 3 + 2];
		// 三角形を構成する頂点座標をベクトルに代入
		XMVECTOR p0 = XMLoadFloat3(&vertices[indices0].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices[indices1].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices[indices2].pos);
		// p0→p1ベクトル、p0→p2ベクトルを計算 （ベクトルの減算）
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);
		// 外積は両方から垂直なベクトル
		XMVECTOR normal = XMVector3Cross(v1, v2);
		// 正規化（長さを1にする)
		normal = XMVector3Normalize(normal);
		// 求めた法線を頂点データに代入
		XMStoreFloat3(&vertices[indices0].normal, normal);
		XMStoreFloat3(&vertices[indices1].normal, normal);
		XMStoreFloat3(&vertices[indices2].normal, normal);
	}

	//Send Vertex Buffer data (Every frame)
	//Get GPU virtual address;

	//XMFLOAT3* vertMap = nullptr;
	Vertex* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	//For every vertex
	for (int i = 0; i < _countof(vertices); i++)
	{
		vertMap[i] = vertices[i]; //Copy the coordinates
	}
	//Unmap vertex buffer
	vertBuff->Unmap(0, nullptr);

	// GPU上のバッファに対応した仮想メモリを取得
	unsigned short* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	// 全インデックスに対して
	for (int i = 0; i < _countof(indices); i++)
	{
		indexMap[i] = indices[i]; // インデックスをコピー
	}
	//Unmap indwx buffer
	indexBuff->Unmap(0, nullptr);
	/////////////////////////////////////
	dx12->cmdList->SetPipelineState(pipelinestate.Get());
	dx12->cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, width, height));
	dx12->cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, width, height));

}

void Mesh::Draw()
{
	//Root Signature setting command
	dx12->cmdList->SetGraphicsRootSignature(rootsignature.Get());
	//Primitive shape setting command	
	dx12->cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	////////////////////////////////////////////////////////////////

	////Depth test
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = dsvHeap->GetCPUDescriptorHandleForHeapStart();
	dx12->cmdList->OMSetRenderTargets(1, &dx12->rtvH, false, &dsvH);
	dx12->cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	//Vertex Array setting command(Vertex Buffer)
	dx12->cmdList->IASetVertexBuffers(0, 1, &vbView);
	dx12->cmdList->IASetIndexBuffer(&ibView);

	ID3D12DescriptorHeap* ppHeaps[] = { basicDescHeap.Get() };
	dx12->cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	dx12->cmdList->SetGraphicsRootDescriptorTable(0, basicDescHeap->GetGPUDescriptorHandleForHeapStart()); // ヒープの先頭が定数バッファ

	D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandle;
	// 定数バッファビュー(先頭)
	gpuDescHandle = basicDescHeap->GetGPUDescriptorHandleForHeapStart();
	dx12->cmdList->SetGraphicsRootDescriptorTable(0, gpuDescHandle);
	// シェーダリソースビュービュー(定数バッファの次)
	gpuDescHandle.ptr += dx12->dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	dx12->cmdList->SetGraphicsRootDescriptorTable(1, gpuDescHandle);

	texture->Set();
	///Draw command
	dx12->cmdList->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);
	/////////////////////////////////////////////////////////////////////////////////////////

	// 3.描画コマンドここまで
}