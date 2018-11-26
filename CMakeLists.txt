cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(WinterUI)
include_directories(include)
set(SRC
		definitions.hh
		observer.hh
		pixmap.hh
		sharedAssets.hh
		signal.hh
		util.hh
		version.hh
		widgets.hh
		WinterUI.hh)
add_executable(${PROJECT_NAME} ${SRC} dummy.cc)
