#include "GameTimer.h"

namespace Core
{
		CrGameTimer::CrGameTimer()
		{
			m_Start = chrono_clock::now();
			m_Timestamp = chrono_clock::now();
		}

		CrGameTimer::~CrGameTimer()
		{
		}

		void CrGameTimer::Reset()
		{
			m_Timestamp = chrono_clock::now();
		}

		double CrGameTimer::GetDelta() const
		{
			auto current = chrono_clock::now();
			auto duration = current - m_Timestamp;
			auto fsec = std::chrono::duration_cast<float_seconds>(duration);
			return fsec.count();
		}

		double CrGameTimer::GetTotal() const
		{
			auto current = chrono_clock::now();
			auto duration = current - m_Start;
			auto fsec = std::chrono::duration_cast<float_seconds>(duration);
			return fsec.count();
		}

	}