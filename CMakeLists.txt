cmake_minimum_required(VERSION 3.13)

project(EmmyLuaLS)

set(CMAKE_CXX_STANDARD 23)

if(APPLE)
	include(CheckCCompilerFlag)

	set(CMAKE_REQUIRED_LINK_OPTIONS "-arch;x86_64")
	check_c_compiler_flag("-arch x86_64" x86_64Supported)
	message("x86_64Supported=${x86_64Supported}")

	set(CMAKE_REQUIRED_LINK_OPTIONS "-arch;arm64")
	check_c_compiler_flag("-arch arm64" arm64Supported)
	message("arm64Supported=${arm64Supported}")
endif ()

add_subdirectory(3rd/fmt-9.1.0)
add_subdirectory(Core/LuaParser)
add_subdirectory(Util)
add_subdirectory(Core)
add_subdirectory(LanguageServer)