cmake_minimum_required(VERSION 3.13)

project(LanguageServer)

add_executable(LanguageServer)

add_dependencies(LanguageServer Core)

target_include_directories(LanguageServer PRIVATE
        ${EmmyLuaLS_SOURCE_DIR}/3rd/asio-1.24.0/include
        ./
        ${EmmyLuaLS_SOURCE_DIR}/3rd/mimalloc-2.0.3/include
        )

target_compile_options(LanguageServer PUBLIC -DASIO_STANDALONE)

target_sources(LanguageServer
        PRIVATE
        main.cpp
        LanguageServer.cpp

        # Config
        Config/ClientConfig.cpp

        # LSP
        LSPHandle/LSPHandle.cpp

        # Session
        Session/IOSession.cpp
        Session/SocketIOSession.cpp
        Session/StandardIOSession.cpp

        # protocol
        Session/Protocol/ProtocolParser.cpp
        Session/Protocol/ProtocolBuffer.cpp

        # vfs
        Service/VirtualFileSystem.cpp

        # service
        Service/Service.cpp
        Service/DiagnosticService.cpp
        Service/CommandService.cpp
        Service/CodeActionService.cpp
        Service/ConfigService.cpp

        # mimalloc
        ${EmmyLuaLS_SOURCE_DIR}/3rd/mimalloc-2.0.3/src/static.c
        )

target_link_libraries(LanguageServer PUBLIC Core)

if (CMAKE_SYSTEM_NAME MATCHES "Windows")
    target_compile_definitions(LanguageServer PUBLIC -D_WIN32_WINNT=0x0601)
elseif (CMAKE_SYSTEM_NAME MATCHES "Linux")
    target_link_libraries(LanguageServer PUBLIC pthread -static-libstdc++ -static-libgcc)
endif ()

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(LanguageServer PRIVATE /W3 /WX)
    target_compile_options(LanguageServer PRIVATE /utf-8)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(LanguageServer PRIVATE -Wall -Werror)
endif ()

install(
        TARGETS LanguageServer
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
)
