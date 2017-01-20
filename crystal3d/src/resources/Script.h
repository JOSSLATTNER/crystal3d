#pragma once
#include "core\Core.h"
#include "resources\ResourceManager.h"

#include <packages/sol/sol.hpp>
#define SOL_BASE(...) sol::base_classes, sol::bases<##__VA_ARGS__>()

namespace Scripting
{
	class CrScript 
		: public Resources::IResource
	{
	public:
		CrScript();

		// Inherited via IResource
		void LoadFromFile(const std::string & a_File, Resources::ResourceCreateInfo * a_Info) override;
		~CrScript() override;

	public:
		void Run();

		template <typename T>
		void BindGlobal(const std::string& a_Name, T a_Value) 
		{
			m_State[a_Name] = a_Value;
		}

		template <typename RET, typename...ARGS>
		RET Invoke(const std::string& a_Name, ARGS...params)
		{
			std::function<RET(ARGS...)> func = m_State[a_Name];
			return func(params...);
		}

		template <typename T>
		static T* HeapAlloc0()
		{
			return new T();
		}

		template <typename T, typename Arg0>
		static T* HeapAlloc1(Arg0 a_Arg0)
		{
			return new T(a_Arg0);
		}

	protected:
		sol::load_result m_Script;
		sol::state m_State;

	};
}
