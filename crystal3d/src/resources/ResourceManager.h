#pragma once
#include "core\Core.h"
#include "interface\Resource.h"

namespace Resources
{
	template<typename T>
	using CrResourcePtr = std::shared_ptr<T>;

	class CrResourceManager
	{
	public:
		CrResourceManager();
		~CrResourceManager();

		template<typename T>
		CrResourcePtr<T> LoadResource(const CrResourceHandle a_Handle)
		{
			auto it = m_Resources.find(hash);

			if (it != m_Resources.end())
				return std::static_pointer_cast<T>(it->second);

			return nullptr;
		}

		template<typename T, typename...TArgs>
		CrResourcePtr<T> LoadResource(const std::string& a_Filename, TArgs&&...a_Args)
		{
			auto hash = this->GenerateHandle(a_Filename);
			auto fnd = this->LoadResource<T>(hash);

			if (fnd != nullptr)
				return fnd;

			auto fullPath = this->ResolvePath(a_Filename);
			auto res = std::make_shared<T>(fullPath, a_Args...);
			auto resBase = std::static_pointer_cast<CrResource>(res);

			//CrResource::SetReosoureHandle()
			resBase->SetResourceHandle(hash);

			m_Models.insert({ hash, resBase });
			return res;
		}

		void FreeResource(const CrResourceHandle a_Handle);
		void FreeAll();

	private:
		const CrResourceHandle GenerateHandle(const std::string& a_String) const;
		const std::string ResolvePath(const std::string& a_FileName) const;

	private:
		std::unordered_map<CrResourceHandle, CrResourcePtr<CrResource>> m_Resources;

	};
}