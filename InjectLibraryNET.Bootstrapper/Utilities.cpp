#include "pch.h"
#include "Utilities.h"
#include <fstream>

bool Utilities::FileExists(const string_t& file_path)
{
	std::ifstream file_stream;
	file_stream.open(file_path);
	return file_stream.good();
}

string_t Utilities::UtilsGetCurrentDirectory(const HMODULE h_module)
{
	char_t host_path[MAX_PATH];
	constexpr int buffer_size = sizeof host_path / sizeof(char_t);
	GetModuleFileNameW(h_module, host_path, buffer_size);

	return GetDirectoryName(host_path);
}

string_t Utilities::GetDirectoryName(string_t file_path)
{
	const auto pos = file_path.find_last_of('\\');
	file_path = file_path.substr(0, pos + 1);
	return file_path;
}
