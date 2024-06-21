# Set compiler flags for the entire project, including Silo
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

# Invoke PYB11Generator to create the IO module
PYB11Generator_add_module(IO)

# Ensure that IO depends on the silo target
add_dependencies(IO silo_lib)

# Link the Silo library to the IO target
target_link_libraries(IO PRIVATE ${CMAKE_BINARY_DIR}/silo/lib/libsilo.a)


configure_file(PyImage.py PyImage.py COPYONLY)