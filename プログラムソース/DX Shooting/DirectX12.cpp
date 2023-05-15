#include"DirectX12.h"

DirectX12::DirectX12():backBuffers(2) {
	dev = nullptr;
	dxgiFactory = nullptr;
	swapchain = nullptr;
	swapchain1 = nullptr;
	cmdAllocator = nullptr;
	cmdList = nullptr;
	cmdQueue = nullptr;
	rtvHeaps = nullptr;
	debugController = nullptr;
	fence = nullptr;
	fenceVal = 0;
	bbIndex = 0;
	barrierDesc = {};
	heapDesc = {};
	rtvH = {};
}

HRESULT DirectX12::Initialize(WNDCLASSEX& wndc, HWND& hwnd) {

#ifdef _DEBUG

	//if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	//{
	//	debugController->EnableDebugLayer();
	//}

	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}

#endif // _DEBUG

	// DXGI �t�@�N�g���[�̐���
	auto result = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));
	// �A�_�v�^�[�̗񋓗p
	std::vector<IDXGIAdapter*> adapters;
	//std::vector<ComPtr<IDXGIAdapter>> adapters;
	// �����ɓ���̖��O�����A�_�v�^�[�I�u�W�F�N�g������
	IDXGIAdapter* tmpAdapter = nullptr;
	//ComPtr<IDXGIAdapter> tmpAdapter;
	for (int i = 0;
		dxgiFactory->EnumAdapters(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND;
		i++)
	{
		adapters.push_back(tmpAdapter); // ���I�z��ɒǉ�����
	}
	for (int i = 0; i < adapters.size(); i++)
	{
		if (!adapters[i])continue;
		DXGI_ADAPTER_DESC adesc{};
		adapters[i]->GetDesc(&adesc); // �A�_�v�^�[�̏����擾
		std::wstring strDesc = adesc.Description; // �A�_�v�^�[��
		//To avoid Microsoft Basic Render Driver, Intel UHD Graphics
		if (strDesc.find(L"Microsoft") == std::wstring::npos && 
			strDesc.find(L"Intel") == std::wstring::npos)
		{
			tmpAdapter = adapters[i]; // �̗p
			break;
		}
	}

	// �Ή����x���̔z��
	D3D_FEATURE_LEVEL levels[] =
	{
	D3D_FEATURE_LEVEL_12_1,
	D3D_FEATURE_LEVEL_12_0,
	D3D_FEATURE_LEVEL_11_1,
	D3D_FEATURE_LEVEL_11_0,
	};
	D3D_FEATURE_LEVEL featureLevel;
	for (int i = 0; i < _countof(levels); i++)
	{
		// �̗p�����A�_�v�^�[�Ńf�o�C�X�𐶐�
		result = D3D12CreateDevice(tmpAdapter, levels[i], IID_PPV_ARGS(&dev));
		if (result == S_OK)
		{
			// �f�o�C�X�𐶐��ł������_�Ń��[�v�𔲂���
			featureLevel = levels[i];
			break;
		}
	}

	// �R�}���h�A���P�[�^�𐶐�
	result = dev->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&cmdAllocator));
	// �R�}���h���X�g�𐶐�
	result = dev->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
		cmdAllocator.Get(), nullptr, IID_PPV_ARGS(&cmdList));

	// �W���ݒ�ŃR�}���h�L���[�𐶐�
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};
	dev->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&cmdQueue));

	// �e��ݒ�����ăX���b�v�`�F�[���𐶐�
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};
	swapchainDesc.Width = 1280;
	swapchainDesc.Height = 720;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // �F���̏���
	swapchainDesc.SampleDesc.Count = 1; // �}���`�T���v�����Ȃ�
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER; // �o�b�N�o�b�t�@�p
	swapchainDesc.BufferCount = 2; // �o�b�t�@����2�ɐݒ�
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // �t���b�v��͔j��
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	
	result = dxgiFactory->CreateSwapChainForHwnd(
		cmdQueue.Get(),
		hwnd,
		&swapchainDesc,
		nullptr,
		nullptr,
		/*(IDXGISwapChain1**)*/&swapchain1);
	//////
	swapchain1.As(&swapchain);
	//////

	// �e��ݒ�����ăf�B�X�N���v�^�q�[�v�𐶐�
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // �����_�[�^�[�Q�b�g�r���[
	heapDesc.NumDescriptors = 2; // ���\��2��
	dev->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeaps));
	// ���\��2���ɂ���

	for (int i = 0; i < 2; i++)
	{
		// �X���b�v�`�F�[������o�b�t�@���擾
		result = swapchain->GetBuffer(i, IID_PPV_ARGS(&backBuffers[i]));
		// �f�B�X�N���v�^�q�[�v�̃n���h�����擾
		//D3D12_CPU_DESCRIPTOR_HANDLE handle =
		//	rtvHeaps->GetCPUDescriptorHandleForHeapStart();
		CD3DX12_CPU_DESCRIPTOR_HANDLE handle =
			CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeaps->GetCPUDescriptorHandleForHeapStart());
		// �����\���ŃA�h���X�������
		handle.ptr += i * dev->GetDescriptorHandleIncrementSize(heapDesc.Type);
		// �����_�[�^�[�Q�b�g�r���[�̐���
		dev->CreateRenderTargetView(
			backBuffers[i].Get(),
			nullptr,
			handle);

	}

	// �t�F���X()�̐���
	result = dev->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));

	return result;
}

void DirectX12::UpdateStart(float color[]) {
	// �o�b�N�o�b�t�@�̔ԍ����擾(2 �Ȃ̂� 0 �Ԃ� 1 ��)
	bbIndex = swapchain->GetCurrentBackBufferIndex();
	// 1.���\�[�X�o���A��ύX
	barrierDesc.Transition.pResource = backBuffers[bbIndex].Get(); // �o�b�N�o�b�t�@���w��
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT; // �\������
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // �`��
	//cmdList->ResourceBarrier(1, &barrierDesc);
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(backBuffers[bbIndex].Get(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	// 2.��ʃN���A�R�}���h��������
	// �����_�[�^�[�Q�b�g�r���[�p�f�B�X�N���v�^�q�[�v�̃n���h�����擾
	rtvH = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
	rtvH.ptr += bbIndex * dev->GetDescriptorHandleIncrementSize(heapDesc.Type);
	//cmdList->OMSetRenderTargets(1, &rtvH, false, nullptr);
	cmdList->ClearRenderTargetView(rtvH, color, 0, nullptr);
}

void DirectX12::UpdateEnd() {
	// 4.���\�[�X�o���A��߂�
	barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET; // �`��
	barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;// �\����
	//cmdList->ResourceBarrier(1, &barrierDesc);
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(backBuffers[bbIndex].Get(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	//////////////////////////////
	//Command flush
	//Cloase the command(a la fin)
	cmdList->Close();
	//�R�}���h���X�g�̎��s
	ID3D12CommandList* cmdLists[] = { cmdList.Get() };// �R�}���h���X�g�̔z��
	cmdQueue->ExecuteCommandLists(1, cmdLists);
	//�R�}���h���X�g�̎��s������҂�
	cmdQueue->Signal(fence.Get(), ++fenceVal);
	if (fence->GetCompletedValue() != fenceVal) {
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}
	cmdAllocator->Reset(); // �L���[���N���A
	cmdList->Reset(cmdAllocator.Get(), nullptr); // �ĂуR�}���h���X�g�𒙂߂鏀��
	//�o�b�t�@���t���b�v(���\�̓��ւ�)
	swapchain->Present(1, 0);
	///////////////////////////////
}