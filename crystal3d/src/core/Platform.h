#pragma once

#if defined _WIN32 || defined _WIN64
#define CR_PLATFORM_WINDOWS

//################
//Disables iterator debugging.
//#define _SECURE_SCL 0
//################

//################
//Prevents min()/max() name clashes with std::numeric_limits<T>::max()
//#define NOMINMAX
//################

#include <Windows.h>
#elif defined __unix__ || defined unix
#define CR_PLATFORM_UNIX
#elif defined __linux__
#define CR_PLATFORM_LINUX
#elif defined __APPLE__ || defined __MACH__
#define CR_PLATFORM_OSX
#else
#error "Platform unsupported!"
#endif

#if defined _MSC_VER
#define CR_COMPILER_MSC
#define CR_DEBUG _DEBUG
#elif defined __GNUC__ || defined __GNUG__
#define CR_COMPILER_GCC
#elif defined __clang__
#define CR_COMPILER_CLANG
#endif