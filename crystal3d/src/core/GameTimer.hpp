#pragma once
#include <chrono>

#define USE_CHRONO_HIGH_RES_CLOCK

#ifdef USE_CHRONO_HIGH_RES_CLOCK
#define chrono_clock std::chrono::high_resolution_clock
#else
#define chrono_clock std::chrono::steady_clock
#endif

namespace Core
{
		class CrGameTimer
		{
		public:
			CrGameTimer()
			{
				m_Start = chrono_clock::now();
				m_Timestamp = chrono_clock::now();
				CrLogSuccess("Game Timer initialized [OK]");
			}

			~CrGameTimer()
			{
			}

			void Reset()
			{
				m_Timestamp = chrono_clock::now();
			}

			template <typename DurationType>
			DurationType GetDelta() const
			{
				auto current = chrono_clock::now();
				auto duration = current - m_Timestamp;
				typedef std::chrono::duration<DurationType> dt;

				auto fsec = std::chrono::duration_cast<dt>(duration);
				return fsec.count();
			}

			template <typename DurationType>
			DurationType GetTotal() const
			{
				auto current = chrono_clock::now();
				auto duration = current - m_Start;
				typedef std::chrono::duration<DurationType> dt;

				auto fsec = std::chrono::duration_cast<dt>(duration);
				return fsec.count();
			}

		private:
			std::chrono::high_resolution_clock::time_point m_Start;
			std::chrono::high_resolution_clock::time_point m_Timestamp;

		};
}