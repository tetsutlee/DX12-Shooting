#pragma once
#include"WinAPI.h"
#include"Input.h"
#include"DirectX12.h"
#include"Texture.h"
#include"Sprite.h"
#include"Camera.h"
#include<DirectXMath.h>
#include<d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
using namespace DirectX;

class Mesh
{
public:
	Mesh(DirectX12* dx12, Input* input);
	HRESULT Initialize(WNDCLASSEX& wndc, HWND& hwnd,  const int width, const int height, std::wstring fileName);
	void Update(XMFLOAT3 scale, XMFLOAT3 rotation, XMFLOAT3 position, const int width, const int height);
	void Draw();
public:
	DirectX12* dx12;
	Input* input;
	Camera* camera;
	Texture* texture;

	ComPtr<ID3D12Resource> vertBuff;
	ComPtr<ID3D12Resource> indexBuff;
	ComPtr<ID3D12Resource> constBuff;

	D3D12_VERTEX_BUFFER_VIEW vbView;// 頂点バッファビューの作成
	D3D12_INDEX_BUFFER_VIEW ibView;

	ComPtr<ID3DBlob> vsBlob;
	ComPtr<ID3DBlob> psBlob;
	ComPtr<ID3DBlob> errorBlob;

	//ID3D12DescriptorHeap* basicHeap;//Basic heap data デスクリプタを生成
	ComPtr<ID3D12DescriptorHeap> basicDescHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE basicHeapHandle;

	//ID3D12DescriptorHeap* dsvHeap;
	ComPtr<ID3D12DescriptorHeap> dsvHeap;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline;//Set graphics pipeline(struct)
	ComPtr<ID3D12RootSignature> rootsignature;
	ComPtr<ID3D12PipelineState> pipelinestate;//Generate pipeline state
	ComPtr<ID3D12PipelineState> pipelinestateWire;

	D3D12_VIEWPORT viewport;
	D3D12_RECT scissorrect;

private:
	int number;
	bool controlSwitch;
	float radius;
	double PI;
	const float topHeight = 1.5f;

	//////Self generate texture
	const int texWidth = 256; // 横方向ピクセル数
	const int texDataCount = texWidth * texWidth; // 配列の要素数
	XMFLOAT4* texturedata = new XMFLOAT4[texDataCount]; //テクスチャデータ配列 ※必ず後で解放する
	//////

	XMMATRIX matProjection; //射影変換行列
	XMMATRIX matWorld; //ワールド変換行列の作成
	XMMATRIX matView; //ビュー変換行列

	XMMATRIX matScale; // スケーリング行列
	XMMATRIX matRot; // 回転行列
	XMMATRIX matTrans; // 平行移動行列

	XMFLOAT3 position[2]; // 座標を変数として持つ
	XMFLOAT3 wallPositon;

	float degrees;
};

