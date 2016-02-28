# TODO: copyright notice

cmake_minimum_required(VERSION 3.4.3)

#######################################################
# Libraries to generate
#######################################################
add_library(ox
    src/ox_cmdline.c
)
target_include_directories(ox PUBLIC include)

#######################################################
# Library unit tests
#######################################################
add_executable(ox_cmdline_test test/ox_cmdline_test.c)
target_link_libraries(ox_cmdline_test ox)
