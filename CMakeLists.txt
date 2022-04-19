cmake_minimum_required(VERSION 3.0)
project(IZP_projekt C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -pedantic")

add_executable(IZP_projekt pwcheck.c)
