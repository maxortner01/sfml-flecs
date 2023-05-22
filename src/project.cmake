file(GLOB_RECURSE GAME_SRC   ${CMAKE_SOURCE_DIR}/src/game/*.cpp)
file(GLOB_RECURSE ENGINE_SRC ${CMAKE_SOURCE_DIR}/src/game/*.cpp)

add_executable(test ${ENGINE_SRC} ${GAME_SRC})

target_include_directories(
    test
    PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/extern/SFML/include
    ${CMAKE_SOURCE_DIR}/extern/flecs/include
)

target_link_directories(
    test
    PUBLIC
    ${CMAKE_SOURCE_DIR}/build/extern/flecs
    ${CMAKE_SOURCE_DIR}/build/extern/SFML/lib
)

target_link_libraries(
    test
    sfml-graphics
    sfml-window
    sfml-system
    flecs
)