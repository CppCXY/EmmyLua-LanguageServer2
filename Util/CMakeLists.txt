cmake_minimum_required(VERSION 3.11)

project(Util)

add_subdirectory(${EmmyLuaLS_SOURCE_DIR}/3rd/uriparser uriparser.out)

add_library(Util STATIC)

add_dependencies(Util fmt)

target_include_directories(Util
        PUBLIC
        ${EmmyLuaLS_SOURCE_DIR}/3rd/uriparser/include
        ${EmmyLuaLS_SOURCE_DIR}/3rd/wildcards/include
        ./

        )

target_sources(Util
        PRIVATE
        CommandLine/CommandLine.cpp
        String/StringUtil.cpp
        String/Utf8.cpp
        File/Url.cpp
        File/FileFinder.cpp
        )


target_link_libraries(Util PUBLIC uriparser fmt)