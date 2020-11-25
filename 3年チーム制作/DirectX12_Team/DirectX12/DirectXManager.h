#pragma once
#include"wrl.h"
#include"d3d12.h"
#include"d3dx12.h"
#include"dxgi1_6.h"
#include"vector"
//シェーダー用
#include"d3dcompiler.h"
//DirectXTex
#include"DirectXTex.h"
#include"Window.h"


#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dxguid.lib")

using namespace Microsoft::WRL;
using namespace DirectX;


class DirectXManager
{
public:
	DirectXManager();
	~DirectXManager();
	void Init(Window* window);
	void Begin();
	void End();
	ID3D12Device* Dev()const;
	ID3D12GraphicsCommandList*CmdList()const;
private:
	//初期化用
	HRESULT result;
	ComPtr<ID3D12Device>dev;
	ComPtr<IDXGIFactory6> dxgiFactory;
	ComPtr<IDXGISwapChain4> swapChain;
	ComPtr<ID3D12CommandAllocator> cmdAllocator;
	ComPtr<ID3D12GraphicsCommandList> cmdList;
	ComPtr<ID3D12CommandQueue> cmdQueue;
	ComPtr<ID3D12DescriptorHeap> rtvHeaps;
	ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal = 0;
	
	
	//バックバッファの番号獲得
	UINT bbIndex; 
	//デプスステンシルビュー
	ComPtr<ID3D12DescriptorHeap>dsvHeap;
	ComPtr<ID3D12Resource> depthBuffer;

	ComPtr<ID3D12DescriptorHeap> basicDescHeap;
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};


	
	
	float r, g, b, a;//色

//定数バッファ初期化
	struct ConstBufferData
	{
		XMFLOAT4 color;//色（RGBA）
		XMMATRIX mat;//3D変換行列
	};

	ConstBufferData* constMap = nullptr;
	ComPtr<ID3D12Resource> constBuff;

};
