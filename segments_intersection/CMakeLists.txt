cmake_minimum_required(VERSION 3.13)
project(B)

set(CMAKE_CXX_STANDARD 17)

include_directories("/Users/n.zakharevich/CLionProjects/cg/boost_1_62_0")
include_directories("/usr/local/Cellar/gmp/6.1.2_2/include")

add_executable(B two_segments_intersection.cpp tests.h)
target_link_libraries(B two_segments_intersection.cpp gmp gmpxx)