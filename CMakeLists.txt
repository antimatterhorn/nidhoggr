cmake_minimum_required(VERSION 3.10)
project(ReactionDiffusion)

# Set the path to the pyb11 executable
set(PYB11GENERATOR_ROOT_DIR ${CMAKE_SOURCE_DIR}/PYB11Generator)
include(${PYB11GENERATOR_ROOT_DIR}/cmake/PYB11Generator.cmake)

# Add the source files
add_subdirectory(src)

