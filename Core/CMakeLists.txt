cmake_minimum_required(VERSION 3.13)

project(Core)

add_library(Core STATIC "")

target_include_directories(Core
        PUBLIC
        ${EmmyLuaLS_SOURCE_DIR}/3rd/nlohmann_json/include
        ./
        )


add_subdirectory(Lib)
add_subdirectory(LuaParser)
add_subdirectory(Workspace)
add_subdirectory(Compilation)

target_link_libraries(
        Core
        PUBLIC
        Util
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(Core PRIVATE /W3 /WX)
    target_compile_options(Core PRIVATE /utf-8)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(Core PRIVATE -Wall -Werror)
endif ()