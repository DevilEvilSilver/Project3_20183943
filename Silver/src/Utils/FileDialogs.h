#pragma once

#include <string>

namespace Silver {

	class FileDialogs
	{
	public:
		// return empty string if canceled
		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter);
	};

}