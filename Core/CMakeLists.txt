cmake_minimum_required(VERSION 3.13)

project(Core)

add_library(Core STATIC "")

target_include_directories(Core
        PUBLIC
        ${EmmyLuaLS_SOURCE_DIR}/3rd/nlohmann_json/include
        PRIVATE
        ./
        ${EmmyLuaLS_SOURCE_DIR}
        )

target_sources(Core
        PRIVATE
        # LSP
        LSP/LSP.cpp

        # LineIndex
        LineIndex/LineIndex.cpp
        LineIndex/LineTypes.cpp
        )

add_subdirectory(LuaParser)
