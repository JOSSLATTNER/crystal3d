#pragma once
#include "core\Core.h"
#include "MemoryArena.hpp"

namespace Core
{
	class CrLinearAllocator
	{
	public:
		CrLinearAllocator(CrMemoryArena& a_Arena)
		{
			m_Start = a_Arena.Begin();
			m_End = a_Arena.End();
			m_Current = m_Start;

			this->Reset();
		}
		~CrLinearAllocator()
		{
		}

		void Reset()
		{
			memset(m_Start, '\0', m_End - m_Start);
		}

		template<typename T>
		CrConstructable<T> Allocate()
		{
			if (m_End - m_Current < sizeof(T))
				throw std::bad_alloc();

			T* ptr = reinterpret_cast<T*>(m_Current);
			m_Current += sizeof(T);
			return { ptr };
		}

	private:
		char* m_Start;
		char* m_End;
		char* m_Current;

	};
}