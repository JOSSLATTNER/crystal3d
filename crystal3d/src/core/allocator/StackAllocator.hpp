#pragma once
#include "core\Core.h"
#include "MemoryArena.hpp"

namespace Core
{
	class CrStackAllocator
	{
	public:
		CrStackAllocator(CrMemoryArena& a_Arena)
			: m_Arena(a_Arena)
		{
			m_Current = a_Arena.Begin();
		}
		~CrStackAllocator()
		{
		}

		template<typename T, typename...Args>
		T* Allocate(Args&&...a_Args)
		{
			//Check if allocation possible
			if (m_Arena.End() - m_Current < sizeof(T))
				throw std::bad_alloc();

			//Increase marker
			m_Current += sizeof(T);
			CrDebugOutput("Allocated %zu bytes.", sizeof(T));

			//Construct memory
			return new (m_Current) T(std::forward<Args>(a_Args)...);
		}

		template<typename T>
		void Deallocate(T* a_Ptr)
		{
			//Check if deallocation possible
			if (m_Current < m_Arena.Begin() + sizeof(T))
				throw std::bad_alloc();

			//Destruct
			a_Ptr->~T();

			//Decrease marker
			m_Current -= sizeof(T);
			CrDebugOutput("Deallocated %zu bytes.", sizeof(T));

			//Reset memory
			m_Arena.Reset(m_Current, m_Current + sizeof(T));
		}

		template<typename T>
		void DeallocateRange(T* t)
		{
			this->Deallocate<T>(t);
		}

		template<typename T, typename...Args>
		void DeallocateRange(T* t, Args*...a_Args)
		{
			this->DeallocateRange(a_Args...);
		}

	private:
		CrMemoryArena& m_Arena;
		CrMemoryPosition m_Current;

	};
}