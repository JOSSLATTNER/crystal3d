#pragma once
#include "core\Core.h"
#include <packages/sol/sol.hpp>

#include "resources\interface\Resource.h"
#include "Framework.h"

namespace Scripting
{
	class CrScript :
		public Resources::CrResource
	{
	public:
		CrScript(const std::string & a_File);
		~CrScript();

	public:
		void Run();

		template <typename T>
		void BindGlobal(const std::string& a_Name, T&& a_Value) 
		{
			m_State[a_Name] = std::move(value);
		}

		template <typename RET, typename...ARGS>
		RET Invoke(const std::string& a_Name, ARGS...params)
		{
			std::function<RET(ARGS...)> func = m_State[a_Name];
			return func(params...);
		}

	protected:
		sol::load_result m_Script;
		sol::state m_State;

	};
}
