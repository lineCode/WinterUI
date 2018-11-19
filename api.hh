#pragma once

#if defined(WINDOWS)
#define WINTERGUI_API __declspec(dllexport)
#elif defined(LINUX)
#define WINTERGUI_API
#endif
