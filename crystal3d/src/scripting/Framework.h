#pragma once
#include <packages/sol/sol.hpp>
#define SOL_BASE(...) sol::base_classes, sol::bases<##__VA_ARGS__>()

namespace Scripting
{
	class CrFramework
	{
	public:
		CrFramework();
		~CrFramework();

		static void RegisterTypes(sol::state& a_State);

	};
}