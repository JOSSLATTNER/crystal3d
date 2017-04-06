#pragma once
#include "core\Core.h"
#include "MemoryArena.hpp"

namespace Core
{
	template<typename T>
	class CrMemory
	{
	public:
		CrMemory(T* a_Ptr)
			: m_Raw(a_Ptr) {};

		template<typename...Args>
		T* Construct(Args...a_Args)
		{	
			return new (m_Raw) T(std::forward<Args>(a_Args)...);
		}

	private:
		T* m_Raw;
	};

	class CrLinearAllocator
	{
	public:
		CrLinearAllocator(char* a_Start, char* a_End)
		{
			m_Start = a_Start;
			m_End = a_End;
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
		CrMemory<T> Allocate()
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