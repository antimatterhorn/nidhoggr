cmake_policy(SET CMP0079 NEW)
cmake_minimum_required(VERSION 3.10)
project(Nidhoggr)

set(CMAKE_CXX_STANDARD 17)  # Request C++17 standard
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # Enforce C++17 requirement (optional)

message("CXX Compiler ID: ${CMAKE_CXX_COMPILER_ID}")

# Set the path to the pyb11 executable
set(PYB11GENERATOR_ROOT_DIR ${CMAKE_SOURCE_DIR}/extern/PYB11Generator)
include(${PYB11GENERATOR_ROOT_DIR}/cmake/PYB11Generator.cmake)

# Include modules for ExternalProject
include(ExternalProject)

# Specify the HDF5 version
set(HDF5_VERSION "1.10.7")
set(HDF5_MAJOR_MINOR "1.10")

# Construct the HDF5 URL
set(HDF5_URL "https://support.hdfgroup.org/ftp/HDF5/releases/hdf5-${HDF5_MAJOR_MINOR}/hdf5-${HDF5_VERSION}/src/hdf5-${HDF5_VERSION}.tar.gz")

# Define an external project for HDF5
ExternalProject_Add(
    hdf5
    URL ${HDF5_URL}
    URL_HASH SHA256=7a1a0a54371275ce2dfc5cd093775bb025c365846512961e7e5ceaecb437ef15
    PREFIX ${CMAKE_BINARY_DIR}/hdf5
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
    BUILD_COMMAND ${CMAKE_COMMAND} --build . --target install
    INSTALL_COMMAND ${CMAKE_COMMAND} --build . --target install
)

# Define where to find the HDF5 library and headers
ExternalProject_Get_Property(hdf5 INSTALL_DIR)
set(HDF5_INCLUDE_DIRS ${INSTALL_DIR}/include)
set(HDF5_LIBRARIES ${INSTALL_DIR}/lib/libhdf5.a)

# Add the include directories globally
include_directories(${HDF5_INCLUDE_DIRS})

# Create an imported target for the HDF5 library
add_library(hdf5_lib STATIC IMPORTED)
set_target_properties(hdf5_lib PROPERTIES IMPORTED_LOCATION ${HDF5_LIBRARIES})

# Ensure hdf5 build happens before any target needing hdf5_lib
add_dependencies(hdf5_lib hdf5)

# Add the source files
add_subdirectory(src/Type)
add_subdirectory(src/Math)
add_subdirectory(src/DataBase)
add_subdirectory(src/Physics)
add_subdirectory(src/Integrators)
add_subdirectory(src/Boundaries)
add_subdirectory(src/Mesh)
add_subdirectory(src/EOS)
add_subdirectory(src/State)
add_subdirectory(src/Trees)
add_subdirectory(src/Utilities)
add_subdirectory(src/Calculators)
add_subdirectory(src/Generation)
add_subdirectory(src/IO)
