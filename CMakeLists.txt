cmake_minimum_required(VERSION 3.10)
project(Nidhoggr)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message("CXX Compiler ID: ${CMAKE_CXX_COMPILER_ID}")

# Include modules for ExternalProject
include(ExternalProject)

# Specify the HDF5 version and download URL
set(HDF5_VERSION "1.12.1")
set(HDF5_URL "https://support.hdfgroup.org/ftp/HDF5/releases/hdf5-1.12/hdf5-${HDF5_VERSION}/src/hdf5-${HDF5_VERSION}.tar.gz")

# Define an external project for HDF5
ExternalProject_Add(
    hdf5
    URL ${HDF5_URL}
    PREFIX ${CMAKE_BINARY_DIR}/hdf5
    CONFIGURE_COMMAND env CFLAGS=-fPIC CXXFLAGS=-fPIC <SOURCE_DIR>/configure --prefix=<INSTALL_DIR> --enable-cxx
    BUILD_COMMAND make
    INSTALL_COMMAND make install
)

# Define where to find the HDF5 library and headers
ExternalProject_Get_Property(hdf5 INSTALL_DIR)
set(HDF5_INCLUDE_DIRS ${INSTALL_DIR}/include)
set(HDF5_C_LIBRARIES ${INSTALL_DIR}/lib/libhdf5.a)
set(HDF5_CPP_LIBRARIES ${INSTALL_DIR}/lib/libhdf5_cpp.a)

# Add the include directories
include_directories(${HDF5_INCLUDE_DIRS})

# Set up the imported target for HDF5 C library
add_library(hdf5_c_lib STATIC IMPORTED)
set_target_properties(hdf5_c_lib PROPERTIES IMPORTED_LOCATION ${HDF5_C_LIBRARIES})

# Set up the imported target for HDF5 C++ library
add_library(hdf5_cpp_lib STATIC IMPORTED)
set_target_properties(hdf5_cpp_lib PROPERTIES IMPORTED_LOCATION ${HDF5_CPP_LIBRARIES})

# Add dependencies to ensure proper build order
add_dependencies(hdf5_c_lib hdf5)
add_dependencies(hdf5_cpp_lib hdf5)

# Set the path to the pyb11 executable
set(PYB11GENERATOR_ROOT_DIR ${CMAKE_SOURCE_DIR}/extern/PYB11Generator)
include(${PYB11GENERATOR_ROOT_DIR}/cmake/PYB11Generator.cmake)

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

# Make IO depend on the HDF5 build
add_dependencies(IO hdf5)
