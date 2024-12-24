# Source files
set(SRC_FILES
    base_game.h
    sliding_puzzle.cpp
    sliding_puzzle.h
    util.h
)

# astar
add_executable(astar astar.cpp ${SRC_FILES})
target_compile_options(astar PUBLIC
    -Wall -Wextra
    $<$<CONFIG:RELEASE>:-O3> $<$<CONFIG:RELEASE>:-DNDEBUG>
    $<$<CONFIG:DEBUG>:-g> $<$<CONFIG:DEBUG>:-DDEBUG>
)

# astar_fast
add_executable(astar_fast astar_fast.cpp ${SRC_FILES})
target_compile_options(astar_fast PUBLIC
    -Wall -Wextra
    $<$<CONFIG:RELEASE>:-O3> $<$<CONFIG:RELEASE>:-DNDEBUG>
    $<$<CONFIG:DEBUG>:-g> $<$<CONFIG:DEBUG>:-DDEBUG>
)

# astar_consistent
add_executable(astar_consistent astar_consistent.cpp ${SRC_FILES})
target_compile_options(astar_consistent PUBLIC
    -Wall -Wextra
    $<$<CONFIG:RELEASE>:-O3> $<$<CONFIG:RELEASE>:-DNDEBUG>
    $<$<CONFIG:DEBUG>:-g> $<$<CONFIG:DEBUG>:-DDEBUG>
)

# astar_fast_template
add_executable(astar_fast_template astar_fast_template.cpp ${SRC_FILES})
target_compile_options(astar_fast_template PUBLIC
    -Wall -Wextra
    $<$<CONFIG:RELEASE>:-O3> $<$<CONFIG:RELEASE>:-DNDEBUG>
    $<$<CONFIG:DEBUG>:-g> $<$<CONFIG:DEBUG>:-DDEBUG>
)

include_directories(.)