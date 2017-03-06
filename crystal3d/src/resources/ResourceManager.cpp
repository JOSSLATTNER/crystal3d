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

	const std::string CrResourceManager::ResolvePath(const std::string& a_FileName) const
	{
		std::wstring wstr(ASSET_PATH);
		const std::string assetPath(wstr.begin(), wstr.end());
		return assetPath + a_FileName;
	}

	void CrResourceManager::FreeResource(const CrResourceHandle a_Handle)
	{
		m_Resources.erase(a_Handle);
	}

	void CrResourceManager::FreeAll()
	{
		m_Resources.clear();
	}

	const CrResourceHandle CrResourceManager::GenerateHandle(const std::string& a_String) const
	{
		std::hash<std::string> hash_fn;
		return hash_fn(a_String);
	}
}