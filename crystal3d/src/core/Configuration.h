#pragma once
#include "core\Core.h"

namespace Core
{
	class CrConfiguration
	{
	public:
		CrConfiguration(const IO::CrPath& a_Filename);
		~CrConfiguration();

		std::string GetStringValue(const std::string& a_Group, const std::string& a_Key);

		template<typename T>
		T GetValue(const std::string& a_Group, const std::string& a_Key)
		{
			//Cast value string into T
			const std::string str = this->GetStringValue(a_Group, a_Key);
			return Util::string_parse<T>(str);
		}

	private:
		std::map<std::string, std::map<std::string, std::string>> m_Keys;
	};
}