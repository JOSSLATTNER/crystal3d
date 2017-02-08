#include "ResourceManager.h"

namespace Resources
{
	CrResourceManager::CrResourceManager()
	{
	}
	CrResourceManager::~CrResourceManager()
	{
	}

	CrResourcePtr<Scripting::CrScript> CrResourceManager::LoadScript(const std::string & a_Filename)
	{
		auto fullPath = this->GetFullPath(a_Filename);
		auto hash = this->GenerateHandle(a_Filename);

		auto it = m_Scripts.find(hash);
		if (it != m_Scripts.end())
			return (*it).second;

		return std::make_shared<Scripting::CrScript>(fullPath);
	}

	CrResourcePtr<CrModel> CrResourceManager::LoadModel(const std::string & a_Filename)
	{
		auto fullPath = this->GetFullPath(a_Filename);
		auto hash = this->GenerateHandle(a_Filename);

		auto it = m_Models.find(hash);
		if (it != m_Models.end())
			return (*it).second;

		return std::make_shared<CrModel>(fullPath);
	}

	const std::string CrResourceManager::GetFullPath(const std::string& a_FileName) const
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