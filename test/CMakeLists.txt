cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

if(WIN32 OR WIN64)
	set(ARCH "x86_64")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_FORTIFY_SOURCE=2 -mmitigate-rop -fstack-protector-strong -fPIE -static -std=c++17 -Wall -Wextra -Wpedantic -Wformat-security -Wno-unused")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--dynamicbase -Wl,--nxcompat -Wl,--export-all-symbols")
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--high-entropy-va -Wl,-z,--relro -Wl,-z,--now")
elseif(UNIX OR APPLE)
	set(ARCH "x86_64")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_FORTIFY_SOURCE=2 -mmitigate-rop -fstack-protector-strong -fPIE -std=c++17 -Wall -Wextra -Wpedantic -Wformat-security -Wno-unused")
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,high-entropy-va -Wl,-z,relro -Wl,-z,now")
endif()

project(UITest)
include_directories(include)
set(SRC
		glad.c
		test.cc)
add_executable(${PROJECT_NAME} ${SRC})
if(WIN32 OR WIN64)
	target_link_libraries(${PROJECT_NAME}
			${CMAKE_SOURCE_DIR}/libs/libSDL2.dll.a
			${CMAKE_SOURCE_DIR}/libs/libpango-1.0.dll.a
			${CMAKE_SOURCE_DIR}/libs/libpangowin32-1.0.dll.a)
elseif(UNIX OR APPLE)
	target_link_libraries(${PROJECT_NAME} dl sdl2 pango cairo pangocairo)
endif()

if(WIN32 OR WIN64)
	add_custom_target(copyAssets COMMAND cmd /c ${CMAKE_SOURCE_DIR}/assets.bat)
	add_dependencies(${PROJECT_NAME} copyAssets)
	add_definitions(-DWINDOWS)
elseif(UNIX)
	add_definitions(-DLINUX)
elseif(APPLE)
	add_definitions(-DOSX)
endif()

if(CMAKE_BUILD_TYPE MATCHES Release)
	set(CMAKE_CXX_FLAGS_RELEASE "-Ofast")
	add_definitions(-DRELEASE)
elseif(CMAKE_BUILD_TYPE MATCHES Debug)
	set(CMAKE_CXX_FLAGS_DEBUG "-ggdb3 -O0")
	add_definitions(-DDEBUG)
endif()
