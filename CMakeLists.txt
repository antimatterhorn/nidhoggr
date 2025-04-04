cmake_minimum_required(VERSION 3.10)
project(Nidhoggr)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

message("CXX Compiler ID: ${CMAKE_CXX_COMPILER_ID}")

# Find and enable OpenMP
find_package(OpenMP REQUIRED)

set(EIGEN3_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/extern/eigen)
include_directories(${EIGEN3_INCLUDE_DIR})


# Set the path to the pyb11 executable
set(PYB11GENERATOR_ROOT_DIR ${CMAKE_SOURCE_DIR}/extern/PYB11Generator)
include(${PYB11GENERATOR_ROOT_DIR}/cmake/PYB11Generator.cmake)

# Include modules for ExternalProject
include(ExternalProject)

# Specify the Silo version
set(SILO_VERSION "4.11.1")
set(SILO_URL "https://github.com/LLNL/Silo/releases/download/4.11.1/silo-4.11.1.tar.xz")

# Define an external project for Silo
ExternalProject_Add(
    silo
    URL ${SILO_URL}
    URL_HASH SHA256=49eddc00304aa4a19074b099559edbdcaa3532c98df32f99aa62b9ec3ea7cee2
    PREFIX ${CMAKE_BINARY_DIR}/silo
    CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=<INSTALL_DIR> --disable-fortran CFLAGS=-fPIC CXXFLAGS=-fPIC
    BUILD_COMMAND make
    INSTALL_COMMAND make install
)

# Define where to find the Silo library and headers
ExternalProject_Get_Property(silo INSTALL_DIR)
set(SILO_INCLUDE_DIRS ${INSTALL_DIR}/include)
set(SILO_LIBRARIES ${INSTALL_DIR}/lib/libsiloh5.a)

# Add the include directories and link the Silo library to your targets
include_directories(${SILO_INCLUDE_DIRS})
add_library(silo_lib STATIC IMPORTED)
set_target_properties(silo_lib PROPERTIES IMPORTED_LOCATION ${SILO_LIBRARIES})

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

# Add dependency on Silo to IO target
add_dependencies(IO silo)
