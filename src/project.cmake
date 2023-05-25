set_property(GLOBAL PROPERTY USE_FOLDERS ON)

file(GLOB_RECURSE GAME_SRC   ${CMAKE_SOURCE_DIR}/src/game/*.cpp)
file(GLOB_RECURSE GAME_INC   ${CMAKE_SOURCE_DIR}/include/game/*.h)

file(GLOB_RECURSE ENGINE_SRC ${CMAKE_SOURCE_DIR}/src/engine/*.cpp)
file(GLOB_RECURSE ENGINE_INC ${CMAKE_SOURCE_DIR}/include/engine/*.h)

# Automatically generate the same folder structure in Visual Studio as we have on disk
macro(GroupSources curdir)
	file(GLOB children RELATIVE ${PROJECT_SOURCE_DIR}/${curdir} ${PROJECT_SOURCE_DIR}/${curdir}/*)
	foreach(child ${children})
		if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/${curdir}/${child})
			GroupSources(${curdir}/${child})
		else()
			string(REPLACE "/" "\\" groupname ${curdir})
            source_group(${groupname} FILES ${PROJECT_SOURCE_DIR}/${curdir}/${child})
		endif()
	endforeach()
endmacro()

set(FOLDER_LIST include src)
file(GLOB children RELATIVE ${PROJECT_SOURCE_DIR}/. ${PROJECT_SOURCE_DIR}/./*)
foreach(child ${children})
	if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/${curdir}/${child})
        if (child IN_LIST FOLDER_LIST)
            GroupSources(${child})
        endif()
	endif()
endforeach()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_executable(test ${ENGINE_SRC} ${ENGINE_INC} ${GAME_SRC} ${GAME_INC})

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