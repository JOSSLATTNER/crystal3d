#pragma once
#include "core\Core.h"

#include "Script.h"
#include "Model.h"
#include "Sha"

namespace Resources
{
	typedef uint64_t CrResourceHandle;

	template<typename T>
	using CrResourcePtr = std::shared_ptr<T>;

	class CrResourceManager
	{

	public:
		CrResourceManager();
		~CrResourceManager();

	public:


		void FreeAll();

		CrResourceHandle GenerateHandle(const std::string& a_String) const;
		std::string GetFullPath(const std::string& a_FileName) const;

	private:
		





	};
}