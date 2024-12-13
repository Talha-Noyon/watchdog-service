cmake_minimum_required(VERSION 3.16)
project(WatchdogApp)
# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SRC
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)
set(GTEST_LIBRARY 
    ${CMAKE_SOURCE_DIR}/include/googletest/build/lib/libgtest.a
    ${CMAKE_SOURCE_DIR}/include/googletest/build/lib/libgtest_main.a
)

include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include/
    ${CMAKE_SOURCE_DIR}/data/
    ${CMAKE_SOURCE_DIR}/test
    ${CMAKE_SOURCE_DIR}/include/googletest/googletest/include
)

set(CONFIG_DIR ${CMAKE_BINARY_DIR}/data)
file(MAKE_DIRECTORY ${CONFIG_DIR})

message(STATUS "CONFIG_DIR: ${CONFIG_DIR}")
configure_file(data/watchdog.json ${CONFIG_DIR}/watchdog.json COPYONLY) 

add_executable(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME} PRIVATE ${GTEST_LIBRARY})

