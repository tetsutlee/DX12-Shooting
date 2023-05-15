#pragma once
#include<vector>
#include<list>
#include<string>
#include<time.h>
#include<d3d12.h>
#include<d3dx12.h>
#include<dxgi1_6.h>
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#include<wrl.h>
using namespace Microsoft::WRL;

class DirectX12 {
public:
	DirectX12();
	HRESULT Initialize(WNDCLASSEX& wndc, HWND& hwnd);
	void UpdateStart(float color[]);
	void UpdateEnd();
public:
	//ID3D12Device* dev;
	ComPtr<ID3D12Device> dev;
	//IDXGIFactory6* dxgiFactory;
	ComPtr<IDXGIFactory6> dxgiFactory;
	//IDXGISwapChain4* swapchain;
	ComPtr<IDXGISwapChain4> swapchain;
	ComPtr<IDXGISwapChain1> swapchain1;
	//ID3D12CommandAllocator* cmdAllocator;
	ComPtr<ID3D12CommandAllocator> cmdAllocator;
	//ID3D12GraphicsCommandList* cmdList;
	ComPtr<ID3D12GraphicsCommandList> cmdList;
	//ID3D12CommandQueue* cmdQueue;
	ComPtr<ID3D12CommandQueue> cmdQueue;
	//ID3D12DescriptorHeap* rtvHeaps;
	ComPtr<ID3D12DescriptorHeap> rtvHeaps;
	//ID3D12Debug* debugController;
	ComPtr<ID3D12Debug> debugController;

	//std::vector<ID3D12Resource*> backBuffers;
	std::vector<ComPtr<ID3D12Resource>> backBuffers;

	//ID3D12Fence* fence;
	ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal;

	UINT bbIndex;
	D3D12_RESOURCE_BARRIER barrierDesc;
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH;
};