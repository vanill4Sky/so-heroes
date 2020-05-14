cmake_minimum_required(VERSION 3.2)

project(so-heroes LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_package(Curses REQUIRED)
find_package(Threads REQUIRED)

include_directories(${CURSES_INCLUDE_DIR})

add_executable(so-heroes src/main.cpp )

target_link_libraries(so-heroes ${CURSES_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})