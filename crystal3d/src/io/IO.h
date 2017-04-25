#pragma once
#ifdef CR_USE_STD_FILESYSTEM
#include <filesystem>

namespace IO
{
	typedef std::experimental::filesystem::path CrPath;
}

namespace std
{
	//Provide std::hash specialization for path
	template<> struct hash<std::experimental::filesystem::path>
	{
		size_t operator()(const std::experimental::filesystem::path& p) const
		{
			return std::experimental::filesystem::hash_value(p);
		}
	};
}
#else
#error "IO framework missing"
#endif