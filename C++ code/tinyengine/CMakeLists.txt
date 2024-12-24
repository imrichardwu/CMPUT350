cmake_minimum_required (VERSION 3.16)

project(tinyengine)

# SDL
add_subdirectory(external/SDL)
add_subdirectory(external/SDL_image)
add_subdirectory(external/SDL_ttf)


add_subdirectory(tinyengine)
add_subdirectory(example)
