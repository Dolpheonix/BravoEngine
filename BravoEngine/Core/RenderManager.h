#pragma once
#include <d3d12.h>
#include <dxgi.h>
#include "Core.h"

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

class RenderManager
{
private:
	RenderManager();
	RenderManager(const RenderManager& rhs) = delete;
	RenderManager& operator=(const RenderManager& rhs) = delete;
public:
	~RenderManager();

public:
	static RenderManager* GetSingleton();

	void OnResize();
	bool InitDirect3D();

private:
	static RenderManager* _Singleton;

	Microsoft::WRL::ComPtr<IDXGIFactory4> _Factory;
	Microsoft::WRL::ComPtr<IDXGISwapChain> _SwapChain;
	Microsoft::WRL::ComPtr<ID3D12Device> _Device;

	Microsoft::WRL::ComPtr<ID3D12Fence> _Fence;

	Microsoft::WRL::ComPtr<ID3D12CommandQueue> _CommandQueue;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> _CommandAllocator;
	Microsoft::WRL::ComPtr<ID3D12CommandList> _CommandList;

	static const int SwapChainBufferCount = 2;

	Microsoft::WRL::ComPtr<ID3D12Resource> _SwapChainBuffer[SwapChainBufferCount];
	Microsoft::WRL::ComPtr<ID3D12Resource> _DepthStencilBuffer;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> _RenderTargetHeap;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> _DepthStencilHeap;

	D3D12_VIEWPORT _Viewport;
	D3D12_RECT _ScissorRect;

	D3D_DRIVER_TYPE _DriverType;
	DXGI_FORMAT _BackbufferFormat;
	DXGI_FORMAT _DepthStencilFormat;

	UINT64 _CurrFence;
	int _CurrentBuffer;
	UINT _RtvDescriptorSize;
	UINT _DsvDescriptorSize;
	UINT _SrvDescriptorSize;


};