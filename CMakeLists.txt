cmake_minimum_required(VERSION 3.0)
project(fucimg)

# set(CMAKE_C_COMPILER "g++")
set(CMAKE_CXX_COMPILER "g++")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -g3 -O2 -fsanitize=address")

file(GLOB SOURCES
    "src/*"
)

include_directories(include)
include_directories(hw01)

add_library(HwBmp hw01/bmp.c)
add_executable(${PROJECT_NAME} ${SOURCES} hw01/bmp.c)

# link_directories(hw01)
target_link_libraries(${PROJECT_NAME} HwBmp)
