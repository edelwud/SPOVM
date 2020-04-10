cmake_minimum_required(VERSION 3.10)
project(AsyncRW)
include(GNUInstallDirs)
add_library(AsyncRW SHARED src/main.cpp)

set_property(TARGET AsyncRW PROPERTY CXX_STANDART 20)

find_package(Curses REQUIRED)
target_link_libraries(AsyncRW ncurses)
TARGET_COMPILE_OPTIONS(AsyncRW INTERFACE ncurses)

SET(THREADS_PREFER_PTHREAD_FLAG TRUE)
FIND_PACKAGE(Threads REQUIRED)
TARGET_LINK_LIBRARIES(AsyncRW Threads::Threads)
TARGET_COMPILE_OPTIONS(AsyncRW INTERFACE Threads::Threads)

TARGET_LINK_LIBRARIES(AsyncRW aio)
