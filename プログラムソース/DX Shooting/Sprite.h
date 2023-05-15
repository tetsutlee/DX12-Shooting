#pragma once
#include"DirectX12.h"
#include"Texture.h"
#include"Input.h"
#include<d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

class Sprite
{
public:
	Sprite(DirectX12* dx12, Input* input);
	~Sprite();
	HRESULT Initialize(WNDCLASSEX& wndc, HWND& hwnd, std::wstring fileName);
	HRESULT Generate(const int width, const int height);
	void PipelineSetCommand(const int width, const int height, float rotate, XMVECTOR position, XMFLOAT3 scale);
	void DrawCommand();

private:
	DirectX12* dx12;
	Input* input;
	Texture* texture;

	ComPtr<ID3DBlob> vsBlob;
	ComPtr<ID3DBlob> psBlob;
	ComPtr<ID3DBlob> errorBlob;
	 
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline;//Set graphics pipeline(struct)

	ComPtr<ID3D12RootSignature> spriteRootsignature;
	ComPtr<ID3D12PipelineState> spritePipelineState;
	ComPtr<ID3D12Resource> spriteVertBuff;
	ComPtr<ID3D12Resource> spriteIndexBuff;
	ComPtr<ID3D12Resource> spriteConstBuff;
	D3D12_VERTEX_BUFFER_VIEW spriteVBView;
	ComPtr<ID3D12DescriptorHeap> basicDescHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE basicHeapHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;

	float spriteRotation;
	XMVECTOR spritePosition;
	XMMATRIX spriteMatScale;
	XMMATRIX spriteMatWorld;
	XMMATRIX spriteMatProjection;
};

