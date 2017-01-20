#pragma once
#include "core\Core.h"
#include "IResource.h"

namespace Resources
{
	typedef uint64_t CrResourceHandle;

	class CrResourceManager
	{

	public:
		CrResourceManager();
		~CrResourceManager();

		template <typename T>
		T* FetchResource(const std::string& a_Path, ResourceCreateInfo* a_Info = nullptr)
		{
			CrResourceHandle handle = GenerateHandle(a_Path);
			if (m_Data.count(handle) > 0)
			{
				return static_cast<T*>(m_Data.at(handle));
			}

			T* pResource = new T();

			pResource->LoadFromFile(a_Path, a_Info);
			m_Data[handle] = pResource;
			return pResource;
		}

	public:
		std::string GetFullPath(const std::string& a_FileName) const;

	private:
		CrResourceHandle GenerateHandle(const std::string& a_String) const;
		std::unordered_map<CrResourceHandle, IResource*> m_Data;

	};
}