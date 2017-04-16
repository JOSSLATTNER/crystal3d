#pragma once

#include "Platform.h"
#include "Singleton.hpp"
#include "Utility.h"

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
#include <mutex>
#include <filesystem>

//###########
//###TYPES###
//###########
#ifdef CR_PLATFORM_WINDOWS
typedef WORD CrWord;
typedef BYTE CrByte;
#endif

//######
//##IO##
//######
namespace IO
{
	typedef std::experimental::filesystem::path CrPath;
}

namespace std
{
	//Provide std::hash specialization for path
	template<> struct hash<std::experimental::filesystem::path>
	{
		size_t operator()(const std::experimental::filesystem::path& p) const
		{
			return std::experimental::filesystem::hash_value(p);
		}
	};
}

//###########
//##LOGGING##
//###########
#ifdef CR_PLATFORM_WINDOWS
static HANDLE ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
static std::once_flag _DebugClearFlag;
static char* _LastFunction = "_";

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

inline void _SetConsoleColor(const ConsoleColor a_Color)
{
	SetConsoleTextAttribute(ConsoleHandle, a_Color);
}

template<typename...Args>
void T_DebugOutput(const std::string& a_Format, Args...a_Args)
{
	std::call_once(_DebugClearFlag, []()
	{
		OutputDebugString("\n###########\nDEBUG SESSION\n###########\n");
	});

	const std::string buff = Util::sprintf_safe(a_Format + "\n", a_Args...);
	OutputDebugString(buff.c_str());
}

//DebugBreak()
#define _CR_DEBUG_BREAK DebugBreak
#endif

inline void _PrintFunction(char* a_Function, char* a_LastFunc)
{
#if CR_DEBUG
	if (strcmp(a_Function, a_LastFunc) != 0)
	{
		//Print current function
		_SetConsoleColor(CONSOLE_COLOR_GREY);
		std::cout << "[" << a_Function << "]" << std::endl;
	}
#endif
}

template<typename...Args>
void T_Log(char* a_Function, ConsoleColor a_Color, const std::string& a_Format, Args...a_Args)
{
	//Print current function
	_PrintFunction(a_Function, _LastFunction);

	//Print message
	_SetConsoleColor(a_Color);
	std::cout << Util::sprintf_safe(a_Format, a_Args...) << std::endl;

	//Remember current function
	_LastFunction = a_Function;
}

template<typename...Args>
bool T_Assert(const bool a_Condition, char* a_Function, const std::string& a_Format, Args...a_Args)
{
	if (!a_Condition)
	{	
		//Print current function
		_PrintFunction(a_Function, _LastFunction);

		//Print message
		_SetConsoleColor(CONSOLE_COLOR_RED);
		std::cerr << Util::sprintf_safe(a_Format, a_Args...) << std::endl;

		//Return true when assertion failed
		return true;
	}
	return false;
}

//Logging
#define CrLog_C(format, color, ...) T_Log(__FUNCTION__, color, format, ##__VA_ARGS__)
#define CrLog(format, ...) CrLog_C(format, CONSOLE_COLOR_WHITE, ##__VA_ARGS__)
#define CrLogSuccess(format, ...) CrLog_C(format, CONSOLE_COLOR_GREEN, ##__VA_ARGS__)
#define CrLogWarning(format, ...) CrLog_C(format, CONSOLE_COLOR_YELLOW, ##__VA_ARGS__)
#define CrLogInfo(format, ...) CrLog_C(format, CONSOLE_COLOR_BLUE, ##__VA_ARGS__)

//Assertion
#define CrAssert(condition, format, ...) \
if(T_Assert(condition, __FUNCTION__, format, ##__VA_ARGS__)) \
	_CR_DEBUG_BREAK();

//Debug
#define CrDebugOutput(format, ...) T_DebugOutput(format, ##__VA_ARGS__)

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