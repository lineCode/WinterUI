cmake_minimum_required(VERSION 3.12.3)

set(TEST ON)

set(ARCH "x86_64")
if (CMAKE_BUILD_TYPE MATCHES RELEASE AND ${TEST})
	set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -mwindows)
endif ()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_FORTIFY_SOURCE=2 -mmitigate-rop -fstack-protector-strong -fPIE -static -std=c++17 -Wall -Wextra -Wformat-security -Wno-unused -Wno-strict-aliasing -Wno-return-local-addr")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--dynamicbase -Wl,--nxcompat -Wl,--export-all-symbols")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--high-entropy-va")

project(WinterGUI)

set(SRC
		api.hh
		glad.c
		definitions.hh
		pixmap.hh
		signal.hh
		util.hh
		widgets.hh
		WinterGUI.hh)

include_directories(include)

if(${TEST})
	add_executable(${PROJECT_NAME} ${SRC} test.cc)
else()
	add_library(${PROJECT_NAME} SHARED ${SRC} dummy.cc)
endif()

target_link_libraries(${PROJECT_NAME}
		${CMAKE_SOURCE_DIR}/libs/libpango-1.0.dll.a
		${CMAKE_SOURCE_DIR}/libs/libpangowin32-1.0.dll.a)

add_definitions(-DFORCE_DISCRETE_GPU)

if(WIN32 OR WIN64)
	add_definitions(-DWINDOWS)
elseif(LINUX)
	add_definitions(-DLINUX)
endif()

if(CMAKE_BUILD_TYPE MATCHES Release)
	set(CMAKE_CXX_FLAGS_RELEASE "-Ofast")
	add_definitions(-DRELEASE)
elseif(CMAKE_BUILD_TYPE MATCHES Debug)
	set(CMAKE_CXX_FLAGS_DEBUG "-ggdb3 -O0")
	add_definitions(-DDEBUG)
endif()
