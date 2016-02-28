# TODO: copyright notice

cmake_minimum_required(VERSION 3.4.3)

#######################################################
# Main CMake file for ox-libs
#######################################################
project(ox-libs)

# Only top level modules should set global compiler flags
if(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
    if(NOT CMAKE_BUILD_TYPE)
        message(STATUS "No build type selected, default to Debug")
        set(CMAKE_BUILD_TYPE "Debug")
    endif()

    if(CMAKE_COMPILER_IS_GNUCC)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --std=gnu99 -Wall -Werror")
        set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -ggdb")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=gnu++11 -Wall -Werror")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ggdb")

        # Use GCC to determine the system architechture
        execute_process(
            COMMAND ${CMAKE_C_COMPILER} -dumpmachine
            OUTPUT_VARIABLE TARGET_ARCH
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
    else()
        message(FATAL_ERROR "Unable to determine target architechture without GCC")
    endif()
endif()

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/${TARGET_ARCH})

#######################################################
# Dependencies
#######################################################

#######################################################
# Subprojects
#######################################################
add_subdirectory(lib-oxica)
add_subdirectory(lib-finance)
