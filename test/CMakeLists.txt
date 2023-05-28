cmake_minimum_required(VERSION 3.15)
project(util_test)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/src)

find_package(GTest REQUIRED)

set(SOURCES
    list_test.cpp
    set_test.cpp
)

add_executable(util_test ${SOURCES})
target_link_libraries(util_test util GTest::GTest GTest::Main)