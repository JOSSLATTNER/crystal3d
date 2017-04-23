#pragma once
#include "core\Core.h"

namespace Core
{
	typedef char* CrMemoryPosition;

	class CrMemoryArena
	{
	public:
		CrMemoryArena(CrMemoryPosition a_Start, CrMemoryPosition a_End)
			: m_Start(a_Start), m_Size(a_End - a_Start), m_Released(false)
		{
			this->Reset();
			CrDebugOutput("Reserved %zu bytes", m_Size);
		}

		CrMemoryArena(const size_t a_Size)
			: m_Released(false)
		{
			this->Reserve(a_Size);
		}

		~CrMemoryArena()
		{
			this->Release();
		}

		void Reserve(const size_t a_Size)
		{
			m_Size = a_Size;
			m_Start = new char[m_Size];
			this->Reset();
			CrDebugOutput("Reserved %zu bytes", m_Size);
		}

		void Reset(CrMemoryPosition a_Start, CrMemoryPosition a_End)
		{
			memset(a_Start, '\0', a_End - a_Start);
		}

		void Reset()
		{
			this->Reset(this->Begin(), this->End());
		}

		void Release()
		{
			if (!m_Released)
			{
				delete[] m_Start;
				m_Released = true;
				CrDebugOutput("Released %zu bytes", m_Size);
			}
		}

		CrMemoryArena Partition(size_t a_Size)
		{
			if (a_Size > m_Size)
				throw std::bad_alloc();

			//Shrink arena
			m_Size -= a_Size;

			//Create new arena
			auto beg = m_Start + m_Size;
			auto end = beg + a_Size;

			return {beg, end};
		}

		CrMemoryPosition Begin()
		{
			return m_Start;
		}

		CrMemoryPosition End()
		{
			return m_Start + m_Size;
		}

		size_t Size()
		{
			return m_Size;
		}

	private:
		bool m_Released;
		size_t m_Size;
		CrMemoryPosition m_Start;
	};
}