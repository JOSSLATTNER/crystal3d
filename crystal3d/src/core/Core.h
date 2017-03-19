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

#ifdef Cr_WINDOWS
static HANDLE WIN32_CONSOLE_HANDLE = GetStdHandle(STD_OUTPUT_HANDLE);

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
void T_Log(const char* function, WORD color, const std::string& format, Args...args)
{
	WIN32_CONSOLE_SET_COLOR(WIN32_COLOR_GREY);
	std::cout << "[" << function << "]" << std::endl;
	
	WIN32_CONSOLE_SET_COLOR(color);
	std::cout << CrStringFormat(format, args...) << std::endl;
}

template<typename...Args>
bool T_Assert(const bool condition, const char* function, const std::string& format, Args...args)
{
	if (!condition)
	{
		WIN32_CONSOLE_SET_COLOR(WIN32_COLOR_GREY);
		std::cerr << "[" << function << "]" << std::endl;

		WIN32_CONSOLE_SET_COLOR(WIN32_COLOR_RED);
		std::cerr << CrStringFormat(format, args...) << std::endl;
		return true;
	}
	return false;
}

#define CrLog_C(format, color, ...) T_Log(__FUNCTION__, color, format, ##__VA_ARGS__)
#define CrLog(format, ...) CrLog_C(format, WIN32_COLOR_WHITE, ##__VA_ARGS__)
#define CrLogSuccess(format, ...) CrLog_C(format, WIN32_COLOR_GREEN, ##__VA_ARGS__)
#define CrLogWarning(format, ...) CrLog_C(format, WIN32_COLOR_YELLOW, ##__VA_ARGS__)
#define CrLogInfo(format, ...) CrLog_C(format, WIN32_COLOR_BLUE, ##__VA_ARGS__)

#define CrAssert(condition, format, ...) \
if(T_Assert(condition, __FUNCTION__, format, ##__VA_ARGS__)) \
	DebugBreak();

#endif

template<typename ... Args>
std::string CrStringFormat(const std::string& format, Args ... args)
{
	size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1;
	std::unique_ptr<char[]> buf(new char[size]);
	snprintf(buf.get(), size, format.c_str(), args ...);
	return std::string(buf.get(), buf.get() + size - 1);
}