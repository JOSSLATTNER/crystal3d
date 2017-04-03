#pragma once
#include "core\Core.h"
#include "core\allocator\LinearAllocator.hpp"

namespace Resources
{
	typedef uint64_t CrResourceHandle;

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

			const std::string fullPath = this->ResolvePath(a_Filename);
			auto res = new T(fullPath, std::forward<TArgs>(a_Args)...);

			m_Resources.insert({ hash, static_cast<void*>(res) });

			CrLog(a_Filename.c_str());
			return res;
		}

		void FreeResource(const CrResourceHandle a_Handle);
		void FreeAll();

	private:
		std::unordered_map<CrResourceHandle, void*> m_Resources;

		const CrResourceHandle GenerateHandle(const std::string& a_String) const;
		const std::string ResolvePath(const std::string& a_FileName) const;

	};
}