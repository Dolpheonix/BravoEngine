#include "Root.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
    PSTR cmdLine, int showCmd)
{
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    try
    {
        EngineRoot* MainApp = EngineRoot::CreateInstance(hInstance);
        if (MainApp->InitEngine() == false)
        {
            return 0;
        }
        return MainApp->RunEngine();
    }
    catch (std::exception& e)
    {
        std::string errstr = std::string(e.what());
        std::wstring errwstr;
        errwstr.assign(errstr.begin(), errstr.end());
        MessageBox(nullptr, errwstr.c_str(), L"HR Failed", MB_OK);
        return 0;
    }
}