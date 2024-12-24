add_library(tinyengine STATIC 
    core/engine.cpp
    core/entity_manager.cpp
    core/exception.cpp
    core/render.cpp
)
target_compile_features(tinyengine PUBLIC cxx_std_17)
target_include_directories(tinyengine PUBLIC ..)
# target_include_directories(tinyengine PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(tinyengine SDL2 SDL2_image SDL2_ttf)
target_compile_options(
    tinyengine PUBLIC 
    "-Wall" "-Wextra" 
    "$<$<CONFIG:DEBUG>:-g>" "$<$<CONFIG:DEBUG>:-fno-omit-frame-pointer>" "$<$<CONFIG:DEBUG>:-O0>" 
    "$<$<CONFIG:RELEASE>:-O3>"
)
