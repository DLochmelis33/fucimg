cmake_minimum_required(VERSION 3.0)
project(fucimg)

# set(CMAKE_C_COMPILER "g++")
set(CMAKE_CXX_COMPILER "g++")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -O3 -DNDEBUG")

file(GLOB SOURCES
    "src/*"
)

include_directories(include)
include_directories(hw01)

add_library(HwBmp hw01/bmp.c)
add_executable(${PROJECT_NAME} ${SOURCES})

# link_directories(hw01)
target_link_libraries(${PROJECT_NAME} HwBmp)
