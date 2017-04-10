#pragma once
#include "core\Core.h"

namespace Core
{
	template<typename T>
	class CrConstructable
	{
	public:
		CrConstructable(T* a_Ptr)
			: m_Raw(a_Ptr) {};

		template<typename...Args>
		T* Construct(Args...a_Args)
		{
			return new (m_Raw) T(std::forward<Args>(a_Args)...);
		}

	private:
		T* m_Raw;
	};

	class CrMemoryArena
	{
	public:
		CrMemoryArena(const size_t a_Size)
			: m_Size(a_Size), m_Released(false)
		{
			m_Start = new char[m_Size];

			CrLog("Allocated %zu bytes.", a_Size);
		}
		~CrMemoryArena()
		{
			this->Release();
		}

		void Release()
		{
			if (!m_Released)
			{
				delete m_Start;
				m_Released = true;
			}
		}

		char* Begin()
		{
			return m_Start;
		}

		char* End()
		{
			return m_Start + m_Size;
		}

	private:
		bool m_Released;
		size_t m_Size;
		char* m_Start;
	};
}