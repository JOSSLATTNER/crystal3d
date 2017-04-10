#include "core\Core.h"
#include <chrono>

#ifndef CR_DEBUG
#define _CR_BENCHMARK(section) \
Profiling::CrBenchmark __bm(section, __FUNCTION__)
#else
#define _CR_BENCHMARK(_)
#endif

namespace Profiling
{
	class CrBenchmark
	{
	public:
		CrBenchmark(const std::string& section, const char* func)
			: m_Section(section)
		{
			CrLogInfo("BENCHMARK '%s' @ %s()", m_Section.c_str(), func);
			m_Start = std::chrono::high_resolution_clock::now();
		}
		~CrBenchmark()
		{
			auto end = std::chrono::high_resolution_clock::now();
			CrLogInfo("BENCHMARK '%s' COMPLETED: %llu,%llusec",
				m_Section.c_str(),
				std::chrono::duration_cast<std::chrono::seconds>(end - m_Start).count(),
				std::chrono::duration_cast<std::chrono::microseconds>(end - m_Start).count());
		}
	private:
		std::string m_Section;
		std::chrono::high_resolution_clock::time_point m_Start;
	};
}