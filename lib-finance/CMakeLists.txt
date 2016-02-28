# TODO: copyright notice

cmake_minimum_required(VERSION 3.4.3)

#######################################################
# Libraries to generate
#######################################################
add_library(finance
	src/finance.c
)

target_include_directories(finance PUBLIC include)
target_link_libraries(finance ox m) # 'm' is the standard math library for UNIX

#######################################################
# Library unit tests
#######################################################
add_executable(finance_test test/finance_test.c)
target_link_libraries(finance_test finance ox)
