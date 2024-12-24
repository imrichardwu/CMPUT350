cmake_minimum_required (VERSION 3.16)

project(quadtree)

# OpenGL
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)


add_executable(main 
    src/main.cpp
)
target_include_directories(main PUBLIC ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS})
target_compile_features(main PUBLIC cxx_std_17)
target_link_libraries(main ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})

