cmake_minimum_required(VERSION 3.13)

project(Test)

add_executable(Test)

add_dependencies(Test Core)

target_include_directories(Test PRIVATE
        ./
        )

target_sources(Test
        PRIVATE
        main.cpp
        )

target_link_libraries(Test PUBLIC Core)