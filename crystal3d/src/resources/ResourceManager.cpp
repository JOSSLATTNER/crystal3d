#include "ResourceManager.h"

namespace Resources
{
	CrResourceManager::CrResourceManager()
	{
	}
	CrResourceManager::~CrResourceManager()
	{
		for (auto& r : m_Data)
		{
			delete r.second;
		}
		m_Data.clear();
	}


	std::string CrResourceManager::GetFullPath(const std::string& a_FileName) const
	{
		std::wstring wstr(ASSET_PATH);
		const std::string assetPath(wstr.begin(), wstr.end());
		return assetPath + a_FileName;
	}

	CrResourceHandle CrResourceManager::GenerateHandle(const std::string& a_String) const
	{
		std::hash<std::string> hash_fn;
		return hash_fn(a_String);
	}
}