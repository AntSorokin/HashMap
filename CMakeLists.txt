cmake_minimum_required(VERSION 3.13)
project(C_Hashmap VERSION 1.0.0)

add_library(Hashmap ${CMAKE_CURRENT_SOURCE_DIR}/src/hashmap.c)
target_include_directories(Hashmap PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)