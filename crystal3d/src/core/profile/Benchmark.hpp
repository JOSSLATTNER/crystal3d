#include "core\Core.h"
#include <chrono>

using namespace std::chrono;

#define CrProfile(section) \
CrBenchmark bm__(section, __FUNCTION__)

class CrBenchmark
{
public:
		CrBenchmark(const std::string& section, const char* func)
    	: m_Section(section)
    {
        std::cout << "###" << std::endl;
				std::cout << "STARTED BENCHMARK '" << m_Section << "' @ " << func << "()" << std::endl;
				std::cout << "###" << std::endl;
				m_Start = std::chrono::high_resolution_clock::now();
    }
    ~CrBenchmark()
    {
        auto end = std::chrono::high_resolution_clock::now();
        
				std::cout << "###" << std::endl;
				std::cout << "BENCHMARK '" << m_Section << "' COMPLETED: ";
				std::cout << std::chrono::duration_cast<seconds>(end - m_Start).count() << ",";
				std::cout << std::chrono::duration_cast<microseconds>(end - m_Start).count() << " sec." << std::endl;
				std::cout << "###" << std::endl;
    }
private:
    std::string m_Section;
    high_resolution_clock::time_point m_Start;
};