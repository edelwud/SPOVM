cmake_minimum_required(VERSION 3.10)
project(ThreadRW)
add_executable(ThreadRW main.cpp)

target_link_libraries(ThreadRW ${CMAKE_DL_LIBS})

SET(THREADS_PREFER_PTHREAD_FLAG TRUE)
FIND_PACKAGE(Threads REQUIRED)
TARGET_LINK_LIBRARIES(ThreadRW Threads::Threads)
TARGET_COMPILE_OPTIONS(ThreadRW INTERFACE Threads::Threads)

if(AIO_INCLUDE_DIR AND AIO_LIBRARIES)
    set(AIO_FIND_QUIETLY TRUE)
endif(AIO_INCLUDE_DIR AND AIO_LIBRARIES)

# include dir
find_path(AIO_INCLUDE_DIR libaio.h)

# finally the library itself
find_library(LIBAIO NAMES aio)
set(AIO_LIBRARIES ${LIBAIO})
# handle the QUIETLY and REQUIRED arguments and set AIO_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(AIO DEFAULT_MSG AIO_LIBRARIES AIO_INCLUDE_DIR)

mark_as_advanced(AIO_LIBRARIES AIO_INCLUDE_DIR)

target_link_libraries(ThreadRW ${AIO_LIBRARIES})