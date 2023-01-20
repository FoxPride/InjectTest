#pragma once

#include <string>
#include "./nethost/nethost.h"
#include "core-setup/hostfxr.h"
#include "core-setup/coreclr_delegates.h"
using string_t = std::basic_string<char_t>;

class CoreCLR
{
	public:

		/**
		 * \param success This value returns 1 if the constructor has successfully ran, else 0.
		 */
		explicit CoreCLR(int* success);
		~CoreCLR() = default;

		/**
		 * \brief Initializes and starts the .NET Core runtime.
		 * \param runtime_config_path The full path to the runtimeconfig file for the assembly to be loaded *.runtimeconfig.json
		 * \return True if the operation succeeded, else false.
		 */
		bool LoadRuntime(const string_t& runtime_config_path);

		/**
		 * \param assembly_path Fully qualified path to assembly.
		 * \param type_name Assembly qualified type name. e.g. "DotNetLib.Lib, DotNetLib"
		 * \param method_name e.g. "Hello"
		 * \param delegate_type_name Assembly qualified delegate type name or null.
		 * \param reserved Extensibility parameter (currently unused and must be 0).
		 * \param delegate Pointer where to store the function pointer result. (i.e. This is a delegate representing the NET Core function)
		 * \return True if the operation succeeded, else false.	 
		 */
		bool LoadAssemblyAndGetFunctionPointer(const char_t* assembly_path, const char_t* type_name, const char_t* method_name, const char_t* delegate_type_name, void* reserved, void** delegate) const;

	private:
		/* HostFXR delegates. */
		hostfxr_initialize_for_runtime_config_fn m_init_fptr_{};
		hostfxr_get_runtime_delegate_fn m_get_delegate_fptr_{};
		hostfxr_close_fn m_close_fptr_{};
		load_assembly_and_get_function_pointer_fn m_load_assembly_and_get_function_pointer_ = nullptr;

		/* Helper functions. */
		static void* LoadCoreLibrary(const char_t* path);
		static void* GetExport(void* h, const char* name);
		load_assembly_and_get_function_pointer_fn GetDotnetLoadAssembly(const char_t* config_path);

		/**  Using the nethost library, discovers the location of hostfxr and retrieves function exports. */
		bool load_hostfxr();
};