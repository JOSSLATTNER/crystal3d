#pragma once

#include "Platform.h"
#include "Singleton.hpp"

#ifdef CR_PLATFORM_WINDOWS
//Disables iterator debugging.
//#define _SECURE_SCL 0
#endif

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

//###########
//###TYPES###
//###########
#ifdef CR_PLATFORM_WINDOWS
typedef WORD CrWord;
typedef BYTE CrByte;
#endif

//###########
//##LOGGING##
//###########
#ifdef CR_PLATFORM_WINDOWS
static HANDLE WIN32_CONSOLE_HANDLE = GetStdHandle(STD_OUTPUT_HANDLE);
static char* CURRENT_FUNC = "_";

#define CONSOLE_COLOR_BLACK 0
#define CONSOLE_COLOR_BLUE 31
#define CONSOLE_COLOR_GREEN 10
#define CONSOLE_COLOR_RED 12
#define CONSOLE_COLOR_GREY 8
#define CONSOLE_COLOR_WHITE 15
#define CONSOLE_COLOR_YELLOW 14

#define CONSOLE_SET_COLOR(code) \
SetConsoleTextAttribute(WIN32_CONSOLE_HANDLE, code);

template<typename...Args>
void T_DebugOutput(const std::string& format, Args...args)
{
	OutputDebugString(CrStringFormat(format, args...).c_str());
}
#endif

template<typename...Args>
void T_Log(char* function, CrWord color, const std::string& format, Args...args)
{
#if CR_DEBUG
	if (strcmp(function, CURRENT_FUNC) != 0)
	{
		CONSOLE_SET_COLOR(CONSOLE_COLOR_GREY);
		std::cout << "[" << function << "]" << std::endl;
	}
#endif
	
	CONSOLE_SET_COLOR(color);
	std::cout << CrStringFormat(format, args...) << std::endl;

	CURRENT_FUNC = function;
}

template<typename...Args>
bool T_Assert(const bool condition, char* function, const std::string& format, Args...args)
{
	if (!condition)
	{
#if CR_DEBUG
		if (strcmp(function, CURRENT_FUNC) != 0)
		{
			CONSOLE_SET_COLOR(CONSOLE_COLOR_GREY);
			std::cerr << "[" << function << "]" << std::endl;
		}
#endif

		CONSOLE_SET_COLOR(CONSOLE_COLOR_RED);
		std::cerr << CrStringFormat(format, args...) << std::endl;
		return true;
	}
	return false;
}

#define CrLog_C(format, color, ...) T_Log(__FUNCTION__, color, format, ##__VA_ARGS__)
#define CrLog(format, ...) CrLog_C(format, CONSOLE_COLOR_WHITE, ##__VA_ARGS__)
#define CrLogSuccess(format, ...) CrLog_C(format, CONSOLE_COLOR_GREEN, ##__VA_ARGS__)
#define CrLogWarning(format, ...) CrLog_C(format, CONSOLE_COLOR_YELLOW, ##__VA_ARGS__)
#define CrLogInfo(format, ...) CrLog_C(format, CONSOLE_COLOR_BLUE, ##__VA_ARGS__)

#define CrAssert(condition, format, ...) \
if(T_Assert(condition, __FUNCTION__, format, ##__VA_ARGS__)) \
	DebugBreak();

#define CrDebugOutput(format, ...) T_DebugOutput(format, ##__VA_ARGS__)

//########
//##UTIL##
//########
#define CrSupressWarning(w, s) \
__pragma(warning( push )) \
__pragma(warning( disable : w )) \
s \
__pragma(warning( pop ))

#define FORWARD_DECL(t) class t
#define BIT_HAS_FLAG(b,f) (b & f) == f

template<typename ... Args>
std::string CrStringFormat(const std::string& format, Args ... args)
{
	size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1;
	std::unique_ptr<char[]> buf(new char[size]);
	snprintf(buf.get(), size, format.c_str(), args ...);
	return std::string(buf.get(), buf.get() + size - 1);
}

//##############
//##EXCEPTIONS##
//##############
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