// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

#include <metahost.h>
#include <mscoree.h>
#include <string>

#pragma comment(lib, "mscoree.lib")


using string_t = std::basic_string<wchar_t>;

// Global Variables for .net framework host
HMODULE thisProcessModule;
ICLRMetaHost* metaHost = nullptr;
ICLRRuntimeInfo* runtimeInfo = nullptr;
ICLRRuntimeHost* runtimeHost = nullptr;

// Loads .net framework runtime for C# dll to be executed
void load_library();
// Unloads .net framework runtime
void unload_library();

string_t get_current_directory(HMODULE h_module);
string_t get_directory_name(string_t file_path);

// Bootstrapper Init Functions
DWORD WINAPI hack_thread(HMODULE h_module);


BOOL APIENTRY DllMain(HMODULE h_module, DWORD ul_reason_for_call, LPVOID lp_reserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        thisProcessModule = h_module;
        CloseHandle(CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(hack_thread), h_module, 0, nullptr));
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

DWORD WINAPI hack_thread(const HMODULE h_module)
{
    //Create Console
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);

    load_library();

    while (true)
    {
        if (GetAsyncKeyState(VK_END) & 1)
        {
            break;
        }
    }

    unload_library();

    fclose(f);
    FreeConsole();
    FreeLibraryAndExitThread(h_module, 0);
}


void load_library()
{
    DWORD p_return_value;

    CLRCreateInstance(CLSID_CLRMetaHost, IID_ICLRMetaHost, reinterpret_cast<LPVOID*>(&metaHost));
    metaHost->GetRuntime(L"v4.0.30319", IID_ICLRRuntimeInfo, reinterpret_cast<LPVOID*>(&runtimeInfo));
    runtimeInfo->GetInterface(CLSID_CLRRuntimeHost, IID_ICLRRuntimeHost, reinterpret_cast<LPVOID*>(&runtimeHost));
    runtimeHost->Start();

    const string_t library_path = get_current_directory(thisProcessModule) + L"InjectLibraryStandard.dll";

    runtimeHost->ExecuteInDefaultAppDomain(library_path.c_str(), L"InjectLibraryStandard.MyClass", L"MyMethod", L"MyParameter", &p_return_value);
}
void unload_library()
{
    if (runtimeInfo != nullptr)
    {
        runtimeInfo->Release();
    }
    if (metaHost != nullptr)
    {
        metaHost->Release();
    }
    if (runtimeHost != nullptr)
    {
        runtimeHost->Release();
    }
}

string_t get_current_directory(const HMODULE h_module)
{
    wchar_t host_path[MAX_PATH];
    constexpr int buffer_size = sizeof host_path / sizeof (wchar_t);
    GetModuleFileNameW(h_module, host_path, buffer_size);

    return get_directory_name(host_path);
}
string_t get_directory_name(string_t file_path)
{
    const auto pos = file_path.find_last_of('\\');
    file_path = file_path.substr(0, pos + 1);
    return file_path;
}
