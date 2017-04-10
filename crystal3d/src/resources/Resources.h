#pragma once
#include "core\Core.h"

namespace Resources
{
	inline std::string ResolvePath(const std::string& a_Filename)
	{
		//std::wstring wstr(ASSET_PATH);
		//const std::string assetPath(wstr.begin(), wstr.end());
		return std::string("C:\\Users\\luis9\\Desktop\\crystal3d\\crystal3d\\assets\\") + a_Filename;
	}
}