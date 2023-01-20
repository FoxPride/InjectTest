#include "pch.h"
#include "CoreCLR.hpp"
#include <iostream>

CoreCLR::CoreCLR(int* success)
{
	if (load_hostfxr())
		*success = 1;

	success = 0;
}

/* Core public functions */
bool CoreCLR::load_hostfxr()
{
	// Get the path to CoreCLR's hostfxr
	char_t buffer[MAX_PATH];
	size_t buffer_size = sizeof(buffer) / sizeof(char_t);
	const int rc = get_hostfxr_path(buffer, &buffer_size, nullptr);
	if (rc != 0)
		return false;

	// Load hostfxr and get desired exports
	void* lib = LoadCoreLibrary(buffer);
	m_init_fptr_ = static_cast<hostfxr_initialize_for_runtime_config_fn>(GetExport(lib, "hostfxr_initialize_for_runtime_config"));
	m_get_delegate_fptr_ = static_cast<hostfxr_get_runtime_delegate_fn>(GetExport(lib, "hostfxr_get_runtime_delegate"));
	m_close_fptr_ = static_cast<hostfxr_close_fn>(GetExport(lib, "hostfxr_close"));

	return (m_init_fptr_ && m_get_delegate_fptr_ && m_close_fptr_);
}

bool CoreCLR::LoadRuntime(const string_t& runtime_config_path)
{
	m_load_assembly_and_get_function_pointer_ = GetDotnetLoadAssembly(runtime_config_path.c_str());
	return m_load_assembly_and_get_function_pointer_ != nullptr;
}

bool CoreCLR::LoadAssemblyAndGetFunctionPointer(const char_t* assembly_path, const char_t* type_name,
	const char_t* method_name, const char_t* delegate_type_name, void* reserved, void** delegate) const
{
	const int result = m_load_assembly_and_get_function_pointer_(assembly_path, type_name, method_name, delegate_type_name, reserved, delegate);
	return result == 0 && delegate != nullptr;
};


/* Helpers */
void* CoreCLR::LoadCoreLibrary(const char_t* path)
{
	const HMODULE handle = LoadLibraryW(path);
	return handle;
}

void* CoreCLR::GetExport(void* h, const char* name)
{
	void* f = GetProcAddress(static_cast<HMODULE>(h), name);
	return f;
}

load_assembly_and_get_function_pointer_fn CoreCLR::GetDotnetLoadAssembly(const char_t* config_path)
{
	// Load .NET Core
	hostfxr_handle context = nullptr;
	const int rc = m_init_fptr_(config_path, nullptr, &context);
	if (rc != 0 || context == nullptr)
	{
		m_close_fptr_(context);
		return nullptr;
	}

	// Get the load assembly function pointer
	m_get_delegate_fptr_(context, hdt_load_assembly_and_get_function_pointer, reinterpret_cast<void**>(&m_load_assembly_and_get_function_pointer_));
	m_close_fptr_(context);
	return m_load_assembly_and_get_function_pointer_;
}