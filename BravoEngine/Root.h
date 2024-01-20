#pragma once

#include "Core/Core.h"
#include "Util/Timer.h"
#include "Core/RenderManager.h"

class EngineRoot
{
private:
	EngineRoot(HINSTANCE hInstance);
	EngineRoot(const EngineRoot& rhs) = delete;
	EngineRoot& operator=(const EngineRoot& rhs) = delete;
public:
	~EngineRoot();
public:
	LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	static EngineRoot* CreateInstance(HINSTANCE hInstance);
	static EngineRoot* GetInstance();

	bool InitEngine();
	int RunEngine();

	void Update(Timer& gt);
	void Draw(Timer& gt);

	bool InitWindow();

	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);
private:
	static EngineRoot* _Singleton;

	HINSTANCE _hInstance;
	HWND _hWindow;

	// Manager
	RenderManager* _RenderManager;

	Timer _GameTimer;

	bool _bPaused;
	bool _bMinimized;
	bool _bMaximized;
	bool _bResizing;

	int _WndWidth = 800;
	int _WndHeight = 600;
};