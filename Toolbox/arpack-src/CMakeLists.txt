cmake_minimum_required(VERSION 3.13.0)

project(arpack Fortran)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../../Libs)

file(GLOB ARPACK_SRC "*.f")

include(../../Driver.cmake)

add_library(arpack_obj OBJECT ${ARPACK_SRC})

add_library(${PROJECT_NAME} ${LIBRARY_TYPE} $<TARGET_OBJECTS:arpack_obj>)

message(STATUS "ARPACK Fortran_FLAGS: ${CMAKE_Fortran_FLAGS}")