#pragma once
#include <chrono>

#define USE_CHRONO_HIGH_RES_CLOCK

#ifdef USE_CHRONO_HIGH_RES_CLOCK
#define chrono_clock std::chrono::high_resolution_clock
#else
#define chrono_clock std::chrono::steady_clock
#endif

typedef std::chrono::duration<double> float_seconds;

namespace Core
{
		class CrGameTimer
		{
		public:
			CrGameTimer();
			~CrGameTimer();

		public:
			void Reset();
			double GetDelta() const;
			double GetTotal() const;

		private:
			std::chrono::high_resolution_clock::time_point m_Start;
			std::chrono::high_resolution_clock::time_point m_Timestamp;

		};
}