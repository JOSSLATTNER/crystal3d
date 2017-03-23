#pragma once

#include "Platform.h"
#include "Singleton.hpp"

#include <vector>
#include <cstdint>
#include <map>
#include <string>
#include <unordered_map>
#include <functional>
#include <iostream>
#include <memory>
#include <random>
#include <set>
#include <fstream>

#define CrSupressWarning(w, s) \
__pragma(warning( push )) \
__pragma(warning( disable : w )) \
s \
__pragma(warning( pop ))

#define FORWARD_DECL(t) class t
#define BIT_HAS_FLAG(b,f) (b & f) == f 

#ifdef CR_PLATFORM_WINDOWS
static HANDLE WIN32_CONSOLE_HANDLE = GetStdHandle(STD_OUTPUT_HANDLE);
static char* CURRENT_FUNC = "_";

#define WIN32_COLOR_BLACK 0
#define WIN32_COLOR_BLUE 31
#define WIN32_COLOR_GREEN 10
#define WIN32_COLOR_RED 12
#define WIN32_COLOR_GREY 8
#define WIN32_COLOR_WHITE 15
#define WIN32_COLOR_YELLOW 14

#define WIN32_CONSOLE_SET_COLOR(code) \
SetConsoleTextAttribute(WIN32_CONSOLE_HANDLE, code);

template<typename...Args>
void T_Log(char* function, WORD color, const std::string& format, Args...args)
{
	if (strcmp(function, CURRENT_FUNC) != 0)
	{
		WIN32_CONSOLE_SET_COLOR(WIN32_COLOR_GREY);
		std::cout << "[" << function << "]" << std::endl;
	}
	
	WIN32_CONSOLE_SET_COLOR(color);
	std::cout << CrStringFormat(format, args...) << std::endl;

	CURRENT_FUNC = function;
}

template<typename...Args>
bool T_Assert(const bool condition, char* function, const std::string& format, Args...args)
{
	if (!condition)
	{
		if (strcmp(function, CURRENT_FUNC) != 0)
		{
			WIN32_CONSOLE_SET_COLOR(WIN32_COLOR_GREY);
			std::cerr << "[" << function << "]" << std::endl;
		}

		WIN32_CONSOLE_SET_COLOR(WIN32_COLOR_RED);
		std::cerr << CrStringFormat(format, args...) << std::endl;
		return true;
	}
	return false;
}

template<typename...Args>
void T_DebugOutput(const std::string& format, Args...args)
{
	OutputDebugString(CrStringFormat(format, args...).c_str());
}

#define CrLog_C(format, color, ...) T_Log(__FUNCTION__, color, format, ##__VA_ARGS__)
#define CrLog(format, ...) CrLog_C(format, WIN32_COLOR_WHITE, ##__VA_ARGS__)
#define CrLogSuccess(format, ...) CrLog_C(format, WIN32_COLOR_GREEN, ##__VA_ARGS__)
#define CrLogWarning(format, ...) CrLog_C(format, WIN32_COLOR_YELLOW, ##__VA_ARGS__)
#define CrLogInfo(format, ...) CrLog_C(format, WIN32_COLOR_BLUE, ##__VA_ARGS__)

#define CrAssert(condition, format, ...) \
if(T_Assert(condition, __FUNCTION__, format, ##__VA_ARGS__)) \
	DebugBreak();

#define CrDebugOutput(format, ...) T_DebugOutput(format, ##__VA_ARGS__)
#endif

template<typename ... Args>
std::string CrStringFormat(const std::string& format, Args ... args)
{
	size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1;
	std::unique_ptr<char[]> buf(new char[size]);
	snprintf(buf.get(), size, format.c_str(), args ...);
	return std::string(buf.get(), buf.get() + size - 1);
}

class CrException
	: public std::exception
{
public:
	CrException(const std::string& message)
		: message_(message) {}

	const char* what() const noexcept override
	{
		return message_.c_str(); 
	}
private:
	const std::string message_;
};