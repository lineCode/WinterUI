#pragma once

#if defined(WINDOWS)
#define BEAST_API __declspec(dllexport)
#elif defined(LINUX)
#define BEAST_API
#endif
