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
		glad.c
		WinterGUI.hh
		widgets.cc widgets.hh signal.cc signal.hh test.cc util.hh)

include_directories(include)

if(${TEST})
	add_executable(${PROJECT_NAME} ${SRC})
else()
	add_library(${PROJECT_NAME} SHARED ${SRC})
endif()

target_link_libraries(${PROJECT_NAME}
		${CMAKE_SOURCE_DIR}/libs/libBeast.a
		${CMAKE_SOURCE_DIR}/libs/libpng.a
		${CMAKE_SOURCE_DIR}/libs/libz.a
		${CMAKE_SOURCE_DIR}/libs/libfreetype.a
		${CMAKE_SOURCE_DIR}/libs/libharfbuzz.a)

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
