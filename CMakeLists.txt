cmake_minimum_required(VERSION 3.10)
project(Nidhoggr)

set(CMAKE_CXX_STANDARD 17)  # Request C++17 standard
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # Enforce C++17 requirement (optional)


# Set the path to the pyb11 executable
set(PYB11GENERATOR_ROOT_DIR ${CMAKE_SOURCE_DIR}/extern/PYB11Generator)
include(${PYB11GENERATOR_ROOT_DIR}/cmake/PYB11Generator.cmake)

# Add the source files
add_subdirectory(src/Math)
add_subdirectory(src/Mesh)
add_subdirectory(src/Hydro)