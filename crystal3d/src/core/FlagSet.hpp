#pragma once
#include "core\Core.h"

namespace Core
{
	template <class TENUM, size_t SIZE>
	class CrFlagSet
	{
	public:
		//using underlying_t = typename std::underlying_type<TENUM>::type;
		CrFlagSet() = default;

		CrFlagSet(std::initializer_list<TENUM> a_Flags)
		{
			for (auto& f : a_Flags)
				this->Set(f);
		}

		CrFlagSet& Set(TENUM a_Flag)
		{
			m_Flags.set(a_Flag);
			return *this;
		}

		bool Test(TENUM a_Flag)
		{
			return m_Flags.test(a_Flag);
		}

		CrFlagSet& operator|=(TENUM a_Flag)
		{
			return this->Set(a_Flag);
		}
	private:
		std::bitset<SIZE> m_Flags;
	};
}