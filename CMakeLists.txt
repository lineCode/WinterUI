cmake_minimum_required(VERSION 3.0)

if(WIN32 OR WIN64)
	set(ARCH "x86_64")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_FORTIFY_SOURCE=2 -mmitigate-rop -fstack-protector-strong -fPIE -static -std=c++17 -Wall -Wextra -Wpedantic -Wformat-security -Wno-unused")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--dynamicbase -Wl,--nxcompat -Wl,--export-all-symbols")
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--high-entropy-va")
elseif(LINUX OR APPLE)
	set(ARCH "x86_64")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_FORTIFY_SOURCE=2 -mmitigate-rop -fstack-protector-strong -fPIE -static -std=c++17 -Wall -Wextra -Wpedantic -Wformat-security -Wno-unused")
endif()

project(WinterUI)
include_directories(include)
set(SRC
		definitions.hh
		glad.c
		observer.hh
		pixmap.hh
		sharedAssets.hh
		signal.hh
		util.hh
		widgets.hh
		WinterUI.hh)
add_executable(${PROJECT_NAME} ${SRC} test.cc)
target_link_libraries(${PROJECT_NAME}
		${CMAKE_SOURCE_DIR}/libs/libpango-1.0.dll.a
		${CMAKE_SOURCE_DIR}/libs/libpangowin32-1.0.dll.a)

if(WIN32 OR WIN64)
	add_definitions(-DWINDOWS)
elseif(LINUX)
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
