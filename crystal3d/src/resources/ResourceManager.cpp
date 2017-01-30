#include "ResourceManager.h"

namespace Resources
{
	CrResourceManager::CrResourceManager()
	{
	}
	CrResourceManager::~CrResourceManager()
	{
		this->FreeAll();
	}

	std::string CrResourceManager::GetFullPath(const std::string& a_FileName) const
	{
		std::wstring wstr(ASSET_PATH);
		const std::string assetPath(wstr.begin(), wstr.end());
		return assetPath + a_FileName;
	}

	void CrResourceManager::FreeAll()
	{
		m_Data.clear();
	}

	CrResourceHandle CrResourceManager::GenerateHandle(const std::string& a_String) const
	{
		std::hash<std::string> hash_fn;
		return hash_fn(a_String);
	}
}