set(SERVER_NAME Server)
set(ROOT  ../)

include_directories(../Common)
set(COMMON_FILES_LIST)

foreach(X IN LISTS COMMON_FILES)
    string(CONCAT X ${ROOT} ${X})
    LIST(APPEND COMMON_FILES_LIST ${X})
endforeach()

add_library(${SERVER_NAME} STATIC
        "Header/IServer.h"
        "Header/Server.h"
        "Header/Session.h"
        "Source/IServer.cpp"
        "Source/Server.cpp"
        "Source/Session.cpp"
        ${COMMON_FILES_LIST}
        )

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(Server_Runner main.cpp)

target_link_libraries(Server_Runner ${SERVER_NAME})