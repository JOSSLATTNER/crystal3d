#pragma once
#include "core\Core.h"
#include "interface\Resource.h"
#include "core\allocator\LinearAllocator.hpp"

namespace Resources
{
	class CrResourceManager
	{
	public:
		CrResourceManager();
		~CrResourceManager();

		template<typename T>
		T* LoadResource_(const CrResourceHandle a_Handle)
		{
			auto it = m_Resources.find(a_Handle);

			if (it != m_Resources.end())
				return static_cast<T*>(it->second);

			return nullptr;
		}

		template<typename T, typename...TArgs>
		T* LoadResource(const std::string& a_Filename, TArgs&&...a_Args)
		{
			auto hash = this->GenerateHandle(a_Filename);
			auto fnd = this->LoadResource_<T>(hash);

			if (fnd != nullptr)
				return fnd;

			auto fullPath = this->ResolvePath(a_Filename);
			auto res = new T(fullPath, std::forward<TArgs>(a_Args)...);
			auto resBase = static_cast<CrResource*>(res);

			//CrResource::SetReosoureHandle()
			resBase->SetResourceHandle(hash);

			m_Resources.insert({ hash, resBase });
			return res;
		}

		void FreeResource(const CrResourceHandle a_Handle);
		void FreeAll();

	private:
		std::unordered_map<CrResourceHandle, CrResource*> m_Resources;

		const CrResourceHandle GenerateHandle(const std::string& a_String) const;
		const std::string ResolvePath(const std::string& a_FileName) const;

	};
}