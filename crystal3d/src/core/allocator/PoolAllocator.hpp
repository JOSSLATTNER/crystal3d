#pragma once
#include "MemoryArena.hpp"

namespace Core
{
	struct CrPoolHeader
	{
		bool used;
	};

	template<typename T>
	class CrPoolAllocator
	{
	public:
		CrPoolAllocator(CrMemoryArena& a_Arena)
			: m_Arena(a_Arena)
		{
			m_MaxBuckets = m_Arena.Size() / sizeof(T);
			m_Data = m_Arena.Begin() + sizeof(CrPoolHeader) * m_MaxBuckets;

			std::uninitialized_fill_n(m_Arena.Begin(), m_MaxBuckets, false);
		}

		~CrPoolAllocator()
		{
		}

		template<typename...Args>
		T* Allocate(Args&&...a_Args)
		{
			int index = this->FindEmptyBucket();
			if (index == -1)
				throw CrException("No empty bucket found.");

			//Find bucket header
			auto header = this->GetHeader(index);

			//Set flag
			header->used = true;

			//Find bucket data pointer
			T* ptr = reinterpret_cast<T*>(m_Data + sizeof(T) * index);

			//Construct
			return new (ptr) T(std::forward<Args>(a_Args)...);
		}

		void Deallocate(T* a_Ptr)
		{
			if (a_Ptr < m_Data || a_Ptr > m_Arena.End())
				throw std::bad_alloc();

			//Find index
			size_t index = (a_Ptr - m_Data) / sizeof(T);

			//Set flag
			auto header = this->GetHeader(index);
			header->used = false;

			//Reset memory
			//Note: stride of ptr is T hence +1
			m_Arena.Reset(a_Ptr, a_Ptr + 1)
		}

	private:
		int FindEmptyBucket()
		{
			CrPoolHeader* header = reinterpret_cast<CrPoolHeader*>(m_Arena.Begin());
			for (size_t i = 0; i < m_MaxBuckets; i++)
			{
				if (!header->used)
					return i;
				++header;
			}
			return -1;
		}

		CrPoolHeader* GetHeader(size_t a_Index)
		{
			CrAssert(a_Index > 0 && a_Index < m_MaxBuckets, "Invalid header index");

			CrMemoryPosition pos = m_Arena.Begin() + sizeof(CrPoolHeader) * a_Index;
			return reinterpret_cast<CrPoolHeader*>(pos);
		}

	private:
		CrMemoryPosition m_Data;
		size_t m_MaxBuckets;
		CrMemoryArena& m_Arena;
	};
}
