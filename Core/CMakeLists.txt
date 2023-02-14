cmake_minimum_required(VERSION 3.13)

project(Core)

add_library(Core STATIC "")

target_include_directories(Core
        PUBLIC
        ${EmmyLuaLS_SOURCE_DIR}/3rd/nlohmann_json/include
        ${EmmyLuaLS_SOURCE_DIR}
        ./
        )


add_subdirectory(Lib)
add_subdirectory(LuaParser)

target_link_libraries(
        Core
        PUBLIC
        Util
)