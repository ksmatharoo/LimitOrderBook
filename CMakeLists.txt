cmake_minimum_required (VERSION 2.8)
project (LOB)

#version number of software
set (LOB_VERSION_MAJOR 1)
set (LOB_VERSION_MINOR 0)
set (LOB_VERSION_PATCH 0)

add_definitions(-D_CRT_SECURE_NO_WARNINGS) 
#add_definitions(-DDEV_LINUX)

set(BOOST_ROOT_DIR "$ENV{BOOST_ROOT_DIR}")
include_directories("${BOOST_ROOT_DIR}")
include_directories("include")

file (GLOB inc "incl/*.h")
file(GLOB src "src/main.cpp")
file(GLOB TEST_SRC "src/test.cpp")

source_group("Header Files" FILES ${inc})
source_group("Source Files" FILES ${src})

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

#Release
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)    #Debug
endif()

add_executable (LOB ${src} )
add_executable (LOB_TEST ${TEST_SRC} )



