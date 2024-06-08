PYB11Generator_add_module(Physics)

# Find OpenMP package
find_package(OpenMP)

if(OpenMP_CXX_FOUND)
    # Add OpenMP flags to the compiler
    target_compile_options(Physics PUBLIC ${OpenMP_CXX_FLAGS})
    # Optionally, link with OpenMP library if needed
    target_link_libraries(Physics PUBLIC OpenMP::OpenMP_CXX)
endif()