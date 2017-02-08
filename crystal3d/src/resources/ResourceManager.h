#pragma once
#include "core\Core.h"

#include "Script.h"
#include "Model.h"
#include "Script.h"

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
		CrResourcePtr<Scripting::CrScript> LoadScript(const std::string& a_Filename);
		CrResourcePtr<CrModel> LoadModel(const std::string& a_Filename);
	
	private:
		CrResourceHandle GenerateHandle(const std::string& a_String) const;
		const std::string GetFullPath(const std::string& a_FileName) const;

	private:
		std::unordered_map<CrResourceHandle, CrResourcePtr<CrModel>> m_Models;
		std::unordered_map<CrResourceHandle, CrResourcePtr<Scripting::CrScript>> m_Scripts;

	};
}