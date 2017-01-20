#pragma once

#if defined _WIN32 || defined _WIN64
#define Cr_WINDOWS

#ifndef Cr_DONT_LOAD_API
#include <Windows.h>
#endif
#elif defined __unix__ || defined unix
#define Cr_UNIX
#elif defined __linux__
#define Cr_LINUX
#elif defined __APPLE__ || defined __MACH__
#define Cr_APPLE_OSX
#else
#error "Platform unsupported!"
#endif

#if defined _MSC_VER
#define Cr_COMPILER_MSC
#elif defined __GNUC__ || defined __GNUG__
#define Cr_COMPILER_GCC
#elif defined __clang__
#define Cr_COMPILER_CLANG
#endif