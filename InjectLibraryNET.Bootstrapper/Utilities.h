#pragma once
#include "pch.h"
#include "CoreCLR.hpp"

class Utilities
{
	public:
		/**
		* \brief Returns true if a file with a given path exists.
		* \param file_path The absolute path to the file.
		* \return True if the file exists, else false.
		 */
		static bool FileExists(const string_t& file_path);

		/**
		* \brief Returns the absolute path to the current directory
		* \return The absolute path to the current directory.
		*/
		static string_t UtilsGetCurrentDirectory(HMODULE h_module);

		/**
		* \brief Returns the full path to a directory referenced by a file.
		* \param file_path The full path to a file.
		* \return The full path to a directory referenced by a file.
		*/
		static string_t GetDirectoryName(string_t file_path);
};
