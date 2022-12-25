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
        "Source/IServer.cpp"
        "Source/Server.cpp"
        ${COMMON_FILES_LIST}
        )

add_executable(Server_Runner main.cpp)

target_link_libraries(Server_Runner ${SERVER_NAME})