// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "ShlObj_core.h"
#include "CoreCLR.hpp"

#include <locale>
#include <string>
#include "Utilities.h"
#include <shellapi.h>
#include <sstream>

// Global Variables
CoreCLR* clr;
HMODULE thisProcessModule;

// Bootstrapper Init Functions
DWORD WINAPI load_library_async(LPVOID lpParam);

bool load_library();

/* Entry point */
BOOL APIENTRY DllMain(HMODULE h_module, DWORD  ul_reason_for_call, LPVOID lp_reserved)
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			thisProcessModule = h_module;
			CloseHandle(CreateThread(nullptr, 0, &load_library_async, h_module, 0, nullptr));
			break;

		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
	}
	return TRUE;
}
DWORD WINAPI load_library_async(LPVOID lpParam)
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

	fclose(f);
	FreeConsole();
	FreeLibraryAndExitThread(thisProcessModule, 0);
}

/**
 * \brief Loads the library into the current process given a set of paths.
 * \return false if failed to load library, true otherwise./
 */
bool load_library()
{
	int success = 0;
	clr = new CoreCLR(&success);

	if (!success)
		throw std::exception("Failed to load the `hostfxr` library. Did you copy nethost.dll?");

	const string_t library_path = Utilities::UtilsGetCurrentDirectory(thisProcessModule) + L"InjectLibraryNET.Bootstrapper.dll";
	const string_t runtime_config_path = Utilities::UtilsGetCurrentDirectory(thisProcessModule) + L"net7.0\\Injector.runtimeconfig.json";

	// Load runtime and execute our method.
	if (!clr->LoadRuntime(runtime_config_path))
		throw std::exception("Failed to load .NET Core Runtime");

	const string_t type_name = L"InjectLibraryNET.MyClass, InjectLibraryNET";
	const string_t method_name = L"MyMethod";
	component_entry_point_fn initialize = nullptr;

	if (!clr->LoadAssemblyAndGetFunctionPointer(library_path.c_str(), type_name.c_str(), method_name.c_str(),
		UNMANAGEDCALLERSONLY_METHOD, nullptr, reinterpret_cast<void**>(&initialize)))
	{
		throw std::exception("Failed to load .NET assembly.");
	}

	initialize(nullptr, 0);

	return true;
}