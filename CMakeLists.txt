cmake_minimum_required(VERSION 3.5)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

set(THIRDPARTY_DIR libs)

# macros
include("cmake/lua.cmake")
include("cmake/sokol.cmake")

# cel8
project(cel8)

add_executable(cel8 src/platform/sokol.c src/cel8.h)
target_link_libraries(cel8 lua)
target_link_libraries(cel8 sokol)
target_include_directories(cel8 PUBLIC ${THIRDPARTY_DIR})