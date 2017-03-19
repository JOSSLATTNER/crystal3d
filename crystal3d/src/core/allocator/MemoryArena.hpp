#pragma once
namespace Core
{
	template<size_t Size>
	class CrMemoryArena
	{
	public:
		CrMemoryArena()
		{
			m_Start = new char[Size];
		}
		~CrMemoryArena()
		{
			delete m_Start;
		}

		char* Begin()
		{
			return m_Start;
		}

		char* End()
		{
			return m_Start + Size;
		}

	private:
			char* m_Start;
	};
}