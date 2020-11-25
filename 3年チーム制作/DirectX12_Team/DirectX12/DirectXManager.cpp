#include "DirectXManager.h"

std::vector<ComPtr<ID3D12Resource>>backBuffers(2);

DirectXManager::DirectXManager()
{
}

DirectXManager::~DirectXManager()
{
}

void DirectXManager::Init(Window* window)
{
#pragma region  �A�_�v�^�[�ƃf�o�C�X�̐���
	//�O���t�B�b�N�{�[�h�̃A�_�v�^���
	//DXGI�t�@�N�g���[����
	result = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));
	//�A�_�v�^�[�̗񋓗p
	std::vector<IDXGIAdapter*>adapters;
	//�����ɓ���̖��O�����A�_�v�^�[�I�u�W�F�N�g������
	IDXGIAdapter* tmpAdapter = nullptr;//����ComPtr����
	for (int i = 0;
		dxgiFactory->EnumAdapters(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND; i++)
	{
		adapters.push_back(tmpAdapter);//���I�z��ɒǉ�����
	}
	for (int i = 0; i < adapters.size(); i++)
	{
		DXGI_ADAPTER_DESC adesc{};
		adapters[i]->GetDesc(&adesc);//�A�_�v�^�[��񂩂��Ƃ�
		std::wstring strDesc = adesc.Description;//�A�_�v�^�[��
		//Microsoft Basic render Driver�����
		if (strDesc.find(L"Microsoft") == std::wstring::npos)
		{
			tmpAdapter = adapters[i];//�̗p
			break;
		}
	}

	//�f�o�C�X����
	//�Ή������F���̔z��
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
		//�̗p�����A�_�v�^�[�Ńf�o�C�X���쐬
		result = D3D12CreateDevice(tmpAdapter, levels[i], IID_PPV_ARGS(&dev));
		if (result == S_OK)
		{
			//�f�o�C�X�𐶐��o�������_�Ń��[�v�𔲂���
			featureLevel = levels[i];
			break;
		}
	}
#pragma endregion

#pragma region �R�}���h���X�g����
	//�R�}���h�A���P�[�^�̍쐬
	result = dev->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&cmdAllocator));
	//�R�}���h���X�g����
	result = dev->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
		cmdAllocator.Get(), nullptr, IID_PPV_ARGS(&cmdList));
	//�R�}���h�L���[����
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};
	dev->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&cmdQueue));

#pragma endregion
#pragma region �X���b�v�`�F�[���ƃ����_�[�^�[�Q�b�g

	//�f�X�N���v�^�q�[�v
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//�V�F�[�_���猩����
	descHeapDesc.NumDescriptors = 3;//SRV��CBV�̂Q��
	//��������
	result = dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&basicDescHeap));

	//�萔�o�b�t�@�쐬
	D3D12_HEAP_PROPERTIES cbheapprop{};//�q�[�v�ݒ�
	cbheapprop.Type = D3D12_HEAP_TYPE_UPLOAD;//GPU�ւ̓]���p
	D3D12_RESOURCE_DESC cbresdesc{};//���\�[�X�ݒ�
	cbresdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbresdesc.Width = (sizeof(ConstBufferData) + 0xff)&~0xff;//256�o�C�g�A���C�����g
	cbresdesc.Height = 1;
	cbresdesc.DepthOrArraySize = 1;
	cbresdesc.MipLevels = 1;
	cbresdesc.SampleDesc.Count = 1;
	cbresdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//GPU���\�[�X�̐���

	//���
	result = dev->CreateCommittedResource(
		&cbheapprop,//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbresdesc,//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuff));
	//�萔�o�b�t�@�Ƀf�[�^��]������

	result = constBuff->Map(0, nullptr, (void**)&constMap);//�}�b�s���O
	constMap->color = XMFLOAT4(0, 0, 1, 0.4f);

	//�[�x�l�p�q�[�v�v���p�e�B
	//�[�x�o�b�t�@
	CD3DX12_RESOURCE_DESC depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_D32_FLOAT,
		Window::Window_Width,
		Window::Window_Height,
		1, 0,
		1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

	//�[�x���\�[�X�ݒ�

	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,//�[�x�l�������݂Ɏg��
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		IID_PPV_ARGS(&depthBuffer)
	);
	//�[�x�r���[�p�f�X�N���v�^�q�[�v����
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;//�[�x�r���[�͈��
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;//�f�v�X�X�e���V���r���[

	result = dev->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));
	//�[�x�r���[����
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;//�[�x�l�t�H�[�}�b�g
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dev->CreateDepthStencilView(depthBuffer.Get(), &dsvDesc, dsvHeap->GetCPUDescriptorHandleForHeapStart());
//#pragma region �r���[�|�[�g�̐ݒ肨��уV�U�[��`
//	cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, Window_width, Window_height));
//	cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, Window_width, Window_height));
//#pragma endregion
	//�X���b�v�`�F�[���̍쐬
	ComPtr<IDXGISwapChain1>swapchain1;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	swapChainDesc.Width = Window::Window_Width;
	swapChainDesc.Height = Window::Window_Height;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//�F���̏���
	swapChainDesc.SampleDesc.Count = 1;//�}���`�T���v�����Ȃ�
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;//�o�b�N�o�b�t�@�p
	swapChainDesc.BufferCount = 2;//�o�b�t�@�����Q�ɐݒ�
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;//�t���b�v��͔j��
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	result = dxgiFactory->CreateSwapChainForHwnd(
		cmdQueue.Get(),
		window->GetHWND(),
		&swapChainDesc,
		nullptr,
		nullptr,
		(IDXGISwapChain1**)&swapchain1);

	swapchain1.As(&swapChain);

	//�e��ݒ�����ăf�B�X�N���v�^�q�[�v���쐬

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	heapDesc.NumDescriptors = 3;//6_5
	dev->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeaps));
	//�\���̓���ɂ���


	for (int i = 0; i < 2; i++)
	{
		//�X���b�v�`�F�[������o�b�t�@���l��
		result = swapChain->GetBuffer(i, IID_PPV_ARGS(&backBuffers[i]));
		//�f�B�X�N���v�^�q�[�v�̃n���h�����l��
		CD3DX12_CPU_DESCRIPTOR_HANDLE handle =
			CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeaps->GetCPUDescriptorHandleForHeapStart(),
				i, dev->GetDescriptorHandleIncrementSize(heapDesc.Type));
		//�\�������ŃA�h���X�������

		//�����_�[�^�[�Q�b�g�r���[�̐���
		dev->CreateRenderTargetView(
			backBuffers[i].Get(),
			nullptr,
			handle);
	}


#pragma endregion

#pragma region �t�F���X����
	result = dev->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
#pragma endregion

}

void DirectXManager::Begin()
{
	r = 0.0f;
	g = 0.0f;
	b = 0.0f;
	a = 0.0f;
#pragma region ���\�[�X�o���A�̕ύX���A
	//���\�[�X�o���A�̕ύX�A���A
	
	bbIndex = swapChain->GetCurrentBackBufferIndex();
	//���\�[�X�o���A�ύX
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(backBuffers[bbIndex].Get(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));
#pragma endregion

#pragma region ��ʃN���A�R�}���h

	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
	//�����_�[�^�[�Q�b�g�r���[�p�f�B�X�N���v�^�q�[�v���擾
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH =
		rtvHeaps->GetCPUDescriptorHandleForHeapStart();
	rtvH.ptr += bbIndex * dev->GetDescriptorHandleIncrementSize(descHeapDesc.Type);
	
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = dsvHeap->GetCPUDescriptorHandleForHeapStart();
	cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);
	//�S��ʃN���ARGBA
	float clearColor[] = { r, g, b,a };//�u���[�o�b�N
	cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
#pragma endregion

}

void DirectXManager::End()
{
	//���\�[�X�o���A��߂�
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(backBuffers[bbIndex].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
	//���߂̃N���[�Y
	cmdList->Close();
	//�R�}���h���X�g���s
	ID3D12CommandList*cmdLists[] = { cmdList.Get() };//�R�}���h���X�g�̔z��
	cmdQueue->ExecuteCommandLists(1, cmdLists);
	//�R�}���h���X�g�̎��s�������܂�
	cmdQueue->Signal(fence.Get(), ++fenceVal);
	if (fence->GetCompletedValue() != fenceVal)
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	cmdAllocator->Reset();	//�L���[���N���A
	cmdList->Reset(cmdAllocator.Get(), nullptr);//�ĂуR�}���h���X�g�����߂鏀��
	//�o�b�t�@�t���b�v
	swapChain->Present(1, 0);
}

ID3D12Device * DirectXManager::Dev() const
{
	return dev.Get();
}

ID3D12GraphicsCommandList * DirectXManager::CmdList() const
{
	return cmdList.Get();
}