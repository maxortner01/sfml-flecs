add_executable(test ${CMAKE_SOURCE_DIR}/src/main.cpp)

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