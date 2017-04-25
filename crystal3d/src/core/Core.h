#pragma once
#include "Platform.h"
#include "Utility.h"

#define CR_USE_STD_FILESYSTEM
#include "io\IO.h"

#include <vector>
#include <cstdint>
#include <map>
#include <string>
#include <unordered_map>
#include <functional>
#include <iostream>
#include <memory>
#include <random>
#include <limits>
#include <set>
#include <fstream>
#include <sstream>
#include <bitset>

//#include <thread>
//#include <mutex>

//###########
//###TYPES###
//###########
#ifdef CR_PLATFORM_WINDOWS
typedef WORD CrWord;
typedef BYTE CrByte;
#endif


//##############
//##EXCEPTIONS##
//##############
class CrException
	: public std::exception
{
public:
	template<typename...Args>
	CrException(const std::string& message, Args...a_Args)
		: message_(Util::sprintf_safe(message, a_Args...)) {}

	const char* what() const noexcept override
	{
		return message_.c_str();
	}
private:
	const std::string message_;
};

//###########
//##LOGGING##
//###########
#define _CR_LOG_FUNCTION 0
#define _CR_DEBUG_LOG_FUNCTION 0

#ifdef CR_PLATFORM_WINDOWS
enum ConsoleColor : WORD
{
	CONSOLE_COLOR_BLACK = 0,
	CONSOLE_COLOR_BLUE = 31,
	CONSOLE_COLOR_GREEN = 10,
	CONSOLE_COLOR_RED = 12,
	CONSOLE_COLOR_GREY = 8,
	CONSOLE_COLOR_WHITE = 15,
	CONSOLE_COLOR_YELLOW = 14
};

inline void _CrSetConsoleColor(const ConsoleColor a_Color)
{
	static HANDLE ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(ConsoleHandle, a_Color);
}

template<typename...Args>
void _CrDebugOutput(const std::string& a_Func, const std::string& a_Format, Args...a_Args)
{
#if _CR_DEBUG_LOG_FUNCTION
	const std::string fStr = "[" + a_Func + "]\n";
	OutputDebugString(fStr.c_str());
#endif

	const std::string buff = Util::sprintf_safe(a_Format + "\n", a_Args...);
	OutputDebugString(buff.c_str());
}

#define _CR_DEBUG_BREAK DebugBreak
#endif

inline void _CrPrintFunction(const std::string& a_Function)
{
#if CR_DEBUG && _CR_LOG_FUNCTION
	_CrSetConsoleColor(CONSOLE_COLOR_GREY);
	std::cout << "[" << a_Function << "]" << std::endl;
#endif
}

template<typename...Args>
void _CrLog(const std::string& a_Function, ConsoleColor a_Color, const std::string& a_Format, Args...a_Args)
{
	_CrPrintFunction(a_Function);
	_CrSetConsoleColor(a_Color);
	std::cout << Util::sprintf_safe(a_Format, a_Args...) << std::endl;
}

template<typename...Args>
bool _CrAssert(const bool a_Condition, const std::string& a_Function, const std::string& a_Format, Args...a_Args)
{
	if (!a_Condition)
	{	
		_CrPrintFunction(a_Function);
		_CrSetConsoleColor(CONSOLE_COLOR_RED);
		std::cerr << Util::sprintf_safe(a_Format, a_Args...) << std::endl;
		return true;
	}
	return false;
}

//Logging
#define CrLogColor(format, color, ...) _CrLog(__FUNCTION__, color, format, ##__VA_ARGS__)
#define CrAssert(condition, format, ...) if(_CrAssert(condition, __FUNCTION__, format, ##__VA_ARGS__)) _CR_DEBUG_BREAK();
#define CrLog(format, ...) CrLogColor(format, CONSOLE_COLOR_WHITE, ##__VA_ARGS__)
#define CrLogSuccess(format, ...) CrLogColor(format, CONSOLE_COLOR_GREEN, ##__VA_ARGS__)
#define CrLogWarning(format, ...) CrLogColor(format, CONSOLE_COLOR_YELLOW, ##__VA_ARGS__)
#define CrLogInfo(format, ...) CrLogColor(format, CONSOLE_COLOR_BLUE, ##__VA_ARGS__)
#define CrDebugOutput(format, ...) _CrDebugOutput(__FUNCTION__, format, ##__VA_ARGS__)
#define CrDebugSection(section) CrDebugOutput("\n\n###%s###\n\n", section)