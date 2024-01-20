#include "Core/Allocator.h"
#include "Root.h"
#include <assert.h>
#include <WindowsX.h>

#pragma comment(lib, "Ws2_32.lib")

using Microsoft::WRL::ComPtr;
using namespace std;
using namespace DirectX;

EngineRoot* EngineRoot::_Singleton = nullptr;

LRESULT CALLBACK
MainWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return EngineRoot::GetInstance()->MsgProc(hwnd, msg, wParam, lParam);
}

EngineRoot::EngineRoot(HINSTANCE hInstance) : _hInstance(hInstance)
{
}

EngineRoot::~EngineRoot()
{

}

bool EngineRoot::InitEngine()
{
	_RenderManager = RenderManager::GetSingleton();
	
	assert(_RenderManager);

	bool result = InitWindow();
	if (result == false)
	{
		MessageBox(0, L"InitWindow() Failed", L"Error", 0);
		return false;
	}

	result = _RenderManager->InitDirect3D();
	if (result == false)
	{
		MessageBox(0, L"InitDirect3D() Failed", L"Error", 0);
		return false;
	}

	return true;
}

int EngineRoot::RunEngine()
{
	MSG msg = { 0 };

	_GameTimer.Reset();

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			_GameTimer.Tick();
			if (_bPaused == false)
			{
				Update(_GameTimer);
				Draw(_GameTimer);
			}
			else
			{
				Sleep(100);
			}
		}
	}
	return (int)msg.wParam;
}

void EngineRoot::Update(Timer& gt)
{
}

void EngineRoot::Draw(Timer& gt)
{
}

bool EngineRoot::InitWindow()
{
	WNDCLASS winclass;
	
	winclass.style 			= CS_HREDRAW | CS_VREDRAW;	// ����, ���η� �̵�/�������� �� Redraw
	winclass.lpfnWndProc 	= MainWindowProc;
	winclass.cbClsExtra 	= 0;
	winclass.cbWndExtra 	= 0;
	winclass.hInstance		= _hInstance;
	winclass.hIcon			= LoadIcon(0, IDI_APPLICATION);
	winclass.hCursor		= LoadCursor(0, IDC_ARROW);
	winclass.hbrBackground	= (HBRUSH)GetStockObject(NULL_BRUSH);
	winclass.lpszMenuName 	= 0;
	winclass.lpszClassName	= L"Main Window";

	if(RegisterClass(&winclass) == false)
	{
		MessageBox(0, L"RegisterClass Failed", 0, 0);
		return false;
	}

	RECT R = { 0, 0, _WndWidth, _WndHeight };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);	// 윈도우 창 크기 계산
	int width = R.right - R.left;
	int height = R.bottom - R.top;

	_hWindow = CreateWindow(L"Main Window", L"Caption", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, _hInstance, 0);
	if (_hWindow == nullptr)
	{
		MessageBox(0, L"CreateWindow Failed", 0, 0);
		return false;
	}

	ShowWindow(_hWindow, SW_SHOW);
	UpdateWindow(_hWindow);

	return true;
}

void EngineRoot::OnMouseDown(WPARAM btnState, int x, int y)
{
}

void EngineRoot::OnMouseUp(WPARAM btnState, int x, int y)
{
}

void EngineRoot::OnMouseMove(WPARAM btnState, int x, int y)
{
}

LRESULT EngineRoot::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE)	// ������ ��Ȱ��ȭ
		{
			_bPaused = true;
			_GameTimer.Stop();
		}
		else
		{
			_bPaused = false;
			_GameTimer.Start();
		}

		return 0;

	case WM_SIZE:
		_WndWidth = LOWORD(lParam);
		_WndHeight = HIWORD(lParam);
		switch(wParam)
		{
			case SIZE_MINIMIZED:
				_bPaused = true;
				_bMinimized = true;
				_bMaximized = false;
				break;
			case SIZE_MAXIMIZED:
				_bPaused = false;
				_bMinimized = false;
				_bMaximized = true;
				_RenderManager->OnResize();
			break;
			case SIZE_RESTORED:
				if(_bMinimized == true)
				{
					_bPaused = false;
					_bMinimized = false;
				}
				else if(_bMaximized == true)
				{
					_bPaused = false;
					_bMaximized = false;
				}
				_RenderManager->OnResize();
		}
		return 0;

		case WM_ENTERSIZEMOVE:
			_bPaused = true;
			_bResizing = true;
			_GameTimer.Stop();
			return 0;

		case WM_EXITSIZEMOVE:
			_bPaused = false;
			_bResizing = false;
			_GameTimer.Start();
			_RenderManager->OnResize();
			return 0;

		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;

		case WM_MENUCHAR:
			return MAKELRESULT(0, MNC_CLOSE);

		case WM_GETMINMAXINFO:
			((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
			((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
			return 0;
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
			OnMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return 0;
		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
			OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return 0;
		case WM_MOUSEMOVE:
			OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			return 0;
		case WM_KEYUP:
			if(wParam == VK_ESCAPE)
			{
				PostQuitMessage(0);
			}
			else if(static_cast<int>(wParam) == VK_F2)
			{
				// set4xmsaastate(!origin);
			}
			return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

EngineRoot* EngineRoot::CreateInstance(HINSTANCE hInstance)
{
	if (_Singleton == nullptr)
	{
		EngineRoot singleton(hInstance);
		_Singleton = &singleton;
	}

	return _Singleton;
}

EngineRoot* EngineRoot::GetInstance()
{
	if (_Singleton == nullptr)
	{
		MessageBox(0, L"EngineRoot 싱글턴 객체가 존재하지 않습니다.", L"Error", 0);
	}

	return _Singleton;
}