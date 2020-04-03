cmake_minimum_required(VERSION 3.10)

project(ThreadSync)

add_executable(ThreadSync main.cpp)

set_property(TARGET ThreadSync PROPERTY CXX_STANDART 20)

find_package(Curses REQUIRED)
target_link_libraries(ThreadSync ncurses)
TARGET_COMPILE_OPTIONS(ThreadSync INTERFACE ncurses)

SET(THREADS_PREFER_PTHREAD_FLAG TRUE)
FIND_PACKAGE(Threads REQUIRED)
TARGET_LINK_LIBRARIES(ThreadSync Threads::Threads)
TARGET_COMPILE_OPTIONS(ThreadSync INTERFACE Threads::Threads)