#include "RenderManager.h"
#include "Util/Util.h"

using Microsoft::WRL::ComPtr;
using namespace std;
using namespace DirectX;

RenderManager* RenderManager::_Singleton = nullptr;

RenderManager::RenderManager()
{
	assert(_Singleton == nullptr);

	_CurrFence = 0;
	_CurrentBuffer = 0;
	_RtvDescriptorSize = 0;
	_DsvDescriptorSize = 0;
	_SrvDescriptorSize = 0;
	_DriverType = D3D_DRIVER_TYPE_HARDWARE;
	_BackbufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	_DepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
}

RenderManager::~RenderManager()
{
}

void RenderManager::OnResize()
{
}

bool RenderManager::InitDirect3D(HWND hwnd, int width, int height)
{
#if defined(_DEBUG)
{
	ComPtr<ID3D12Debug> debugController;
	BV_Throw(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
	debugController->EnableDebugLayer();
}
#endif

	_ClientWidth = width;
	_ClientHeight = height;

	BV_Throw(CreateDXGIFactory1(IID_PPV_ARGS(&_Factory)));

	HRESULT hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&_Device));
	// 하드웨어 디바이스 생성 실패 시, WARP 디바이스를 생성
	if (FAILED(hr))
	{
		ComPtr<IDXGIAdapter> pWarpAdapter;
		BV_Throw(_Factory->EnumWarpAdapter(IID_PPV_ARGS(&pWarpAdapter)));
		BV_Throw(D3D12CreateDevice(pWarpAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&_Device)));
	}

	BV_Throw(_Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_Fence)));

	_RtvDescriptorSize = _Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	_DsvDescriptorSize = _Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	_SrvDescriptorSize = _Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msLevel;
	msLevel.Format = _BackbufferFormat;
	msLevel.SampleCount = 4;
	msLevel.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	msLevel.NumQualityLevels = 0;
	BV_Throw(_Device->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &msLevel, sizeof(msLevel)));
	
	_4xMsaaQuality = msLevel.NumQualityLevels;
	assert(_4xMsaaQuality > 0 && "Unexpected MSAA Quality Level");

#ifdef _DEBUG
	// Adapter 로그 출력
#endif
	
	// Command Queue
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};
	cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	BV_Throw(_Device->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&_CommandQueue)));
	// Command Allocator
	BV_Throw(_Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_CommandAllocator)));
	// Command List
	BV_Throw(_Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _CommandAllocator.Get(), nullptr, IID_PPV_ARGS(&_CommandList)));

	// 코드 상 처음에 Reset이 일어나기 때문에 먼저 닫아줌
	_CommandList->Close();

	// Swap Chain
	_SwapChain.Reset();

	DXGI_SWAP_CHAIN_DESC scDesc;
	scDesc.BufferDesc.Width = _ClientWidth;
	scDesc.BufferDesc.Height = _ClientHeight;
	scDesc.BufferDesc.Format = _BackbufferFormat;
	scDesc.BufferDesc.RefreshRate.Numerator = 60;
	scDesc.BufferDesc.RefreshRate.Denominator = 1;	// refresh rate : 60/1
	scDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	scDesc.SampleDesc.Count = _b4xMsaaEnabled ? 4 : 1;
	scDesc.SampleDesc.Quality = _b4xMsaaEnabled ? (_4xMsaaQuality - 1) : 0;
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scDesc.BufferCount = SwapChainBufferCount;
	scDesc.OutputWindow = hwnd;
	scDesc.Windowed = true;
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	scDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	BV_Throw(_Factory->CreateSwapChain(_CommandQueue.Get(), &scDesc, _SwapChain.GetAddressOf()));

	// RTV Heaps
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
	rtvHeapDesc.NumDescriptors = _NumRenderTargetBuffer;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvHeapDesc.NodeMask = 0;
	BV_Throw(_Device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&_RenderTargetHeap)));
	// DSV Heaps
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
	dsvHeapDesc.NumDescriptors = _NumDepthStencilBuffer;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	dsvHeapDesc.NodeMask = 0;
	BV_Throw(_Device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&_DepthStencilHeap)));

	/*TODO*/
	return true;
}

RenderManager* RenderManager::GetSingleton()
{
	if (_Singleton == nullptr)
	{
		_Singleton = new RenderManager();
	}

	return _Singleton;
}