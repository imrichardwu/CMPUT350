cmake_minimum_required (VERSION 3.16)

project(tinyengine)

# SDL
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

add_subdirectory(tinyengine)
add_subdirectory(example)
