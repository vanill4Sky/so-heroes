cmake_minimum_required(VERSION 3.2)

project(so-heroes LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_package(Curses REQUIRED)
find_package(Threads REQUIRED)

include_directories(${CURSES_INCLUDE_DIR})

add_executable(so-heroes src/main.cpp
               src/map.hpp src/map.cpp
               src/army.hpp
               src/treasury.hpp
               src/player.hpp src/player.cpp
               src/gold_mine.hpp src/gold_mine.cpp
               src/dwelling.hpp src/dwelling.cpp
               src/window.hpp src/window.cpp
               src/curses_wrapper.hpp src/curses_wrapper.cpp
               src/visualization.hpp src/visualization.cpp
               src/utils.hpp)

target_link_libraries(so-heroes ${CURSES_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
