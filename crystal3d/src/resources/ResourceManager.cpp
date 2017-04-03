#include "ResourceManager.h"

namespace Resources
{
	CrResourceManager::CrResourceManager()
	{
		//TODO: ALLOCATOR
		CrLogSuccess("ResourceManager initialized [OK]");
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
		auto it = m_Resources.find(a_Handle);
		if (it != m_Resources.end())
		{
			delete it->second;
			m_Resources.erase(a_Handle);
		}
	}

	void CrResourceManager::FreeAll()
	{
		for (auto& resource : m_Resources)
		{
			delete resource.second;
		}
		m_Resources.clear();
	}

	const CrResourceHandle CrResourceManager::GenerateHandle(const std::string& a_String) const
	{
		std::hash<std::string> hash_fn;
		return hash_fn(a_String);
	}
}