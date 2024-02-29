cmake_minimum_required(VERSION 3.10)
project(ReactionDiffusion)

set(CMAKE_CXX_STANDARD 11)

# Add the grid source file
add_library(grid src/grid.cpp)

# Add the executable, linking with the grid library
add_executable(ReactionDiffusion src/main.cpp)
target_link_libraries(ReactionDiffusion grid)