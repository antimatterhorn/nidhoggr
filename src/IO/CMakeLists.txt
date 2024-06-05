# src/IO/CMakeLists.txt

# Invoke PYB11Generator to create the IO module
PYB11Generator_add_module(IO)

# Include HDF5 include directories
include_directories(${HDF5_INCLUDE_DIRS})

# Link the hdf5_lib library to the IO target
target_link_libraries(IO PRIVATE hdf5_c_lib hdf5_cpp_lib)
