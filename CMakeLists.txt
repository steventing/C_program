# Author: Steven Ting  https://github.com/steventing/C_program

cmake_minimum_required(VERSION 2.8)

find_library(LIBRT rt)
if(NOT LIBRT)
    message(FATAL_ERROR "LIBRT not found")
endif(NOT LIBRT)

add_executable(timer_test timer_test.c)
target_link_libraries(timer_test rt)
install(TARGETS timer_test DESTINATION bin)

