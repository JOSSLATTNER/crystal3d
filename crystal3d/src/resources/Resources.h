#pragma once
#include "core\Core.h"

namespace Resources
{
	inline const IO::CrPath ResolvePath(const IO::CrPath& a_Filename)
	{
		//std::wstring wstr(ASSET_PATH);
		//const IO::CrPath assetPath = "C:\\Users\\luis9\\Desktop\\crystal3d\\crystal3d\\assets\\";
		const IO::CrPath assetPath = "C:\\Users\\luis.eltz\\Desktop\\crystal3d\\crystal3d\\assets";
		return assetPath / a_Filename;
	}
}