cmake_minimum_required(VERSION 3.5)
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.15" CACHE STRING "")
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

project(cel8)

set(THIRDPARTY_DIR libs)

# macros
include("cmake/sokol.cmake")

include_directories(src)
include_directories(${THIRDPARTY_DIR})
add_subdirectory(examples)