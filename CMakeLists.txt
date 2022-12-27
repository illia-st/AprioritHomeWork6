cmake_minimum_required(VERSION 3.23)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(TARGET_NAME HomeWork6)


project(${TARGET_NAME})

set(CMAKE_CXX_STANDARD 20)

set(COMMON_FILES
        "Common/CommunicationUnit.h"
        "Common/Core.h"
        "Common/Buffer.h"
        "Common/CommunicationUnit.cpp"
        )
add_subdirectory(Server)
add_subdirectory(Client)
