#pragma once

namespace Core
{
	template <typename T>
	class CrSingleton
	{
	public:
		CrSingleton()
		{
			m_Instance = static_cast <T*> (this);
		}
		~CrSingleton() {}

		static void Release()
		{
			delete m_Instance;
		}

		static T* SharedInstance()
		{
			if (m_Instance == nullptr)
			{
				CrSingleton<T>::m_Instance = new T();
			}

			return m_Instance;
		}

	protected:
		static T* m_Instance;

	};

	template <typename T>
	T* CrSingleton<T>::m_Instance = nullptr;
}