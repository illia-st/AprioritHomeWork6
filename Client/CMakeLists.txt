set(CLIENT_NAME Client)
set(ROOT  ../)

include_directories(../Common)
set(COMMON_FILES_LIST)

foreach(X IN LISTS COMMON_FILES)
    string(CONCAT X ${ROOT} ${X})
    LIST(APPEND COMMON_FILES_LIST ${X})
endforeach()

add_library(${CLIENT_NAME} STATIC
    "Header/IClient.h"
    "Header/Client.h"
    "Header/Host.h"
    "Source/IClient.cpp"
    "Source/Client.cpp"
    "Source/Host.cpp"
    ${COMMON_FILES_LIST}
)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(Client_Runner main.cpp)

target_link_libraries(Client_Runner ${CLIENT_NAME})