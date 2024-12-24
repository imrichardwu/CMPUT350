cmake_minimum_required (VERSION 3.12)

# Set flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(ABSL_PROPAGATE_CXX_STD ON)

project(heuristic_search)

# Build child targets
add_subdirectory(src)
