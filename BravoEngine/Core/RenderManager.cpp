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

bool RenderManager::InitDirect3D()
{
#if defined(_DEBUG)
{
	ComPtr<ID3D12Debug> debugController;
	BV_Throw(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
	debugController->EnableDebugLayer();
}
#endif

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