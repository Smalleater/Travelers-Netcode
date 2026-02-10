cmake_minimum_required(VERSION 3.10)
project(TRA_Engine VERSION 0.1)

# Use C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

link_directories(${COMMON_EXPORT_LIB_DIR})
link_directories(${EXT_LIBS_DIR}/lib)

# Collect all source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Build as a shared library (.dll on Windows, .so on Linux)
add_library(tra_engine SHARED ${SOURCES})

# Set include directories (public headers and internal headers)
target_include_directories(tra_engine
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${EXT_LIBS_DIR}/header
)

# Define export macro for DLL/SO symbol visibility
target_compile_definitions(tra_engine PRIVATE TRA_EXPORTS)

target_link_libraries(tra_engine PUBLIC tra_core)
target_link_libraries(tra_engine PUBLIC tra_ecs)

target_compile_definitions(tra_engine
    PUBLIC
        $<$<CONFIG:Debug>:_DEBUG>
        $<$<CONFIG:Release>:NDEBUG>
)

add_custom_command(TARGET tra_engine POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy_if_different 
    $<TARGET_FILE:tra_engine> ${COMMON_EXPORT_LIB_DIR}/)

add_custom_command(TARGET tra_engine POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy_if_different 
    $<TARGET_LINKER_FILE:tra_engine> ${COMMON_EXPORT_LIB_DIR}/)

file(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/TRA/netcode/engine/*")
add_custom_command(TARGET tra_engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${HEADER_FILES} ${COMMON_EXPORT_INCLUDE_DIR}/engine/)

file(GLOB HEADER_FILES "${COMMON_INCLUDE_DIR}/TRA/*")
add_custom_command(TARGET tra_engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${HEADER_FILES} ${COMMON_EXPORT_COMMON_INCLUDE_DIR}/)

if(WIN32)
    add_library(tra_ecs SHARED IMPORTED)
    set_target_properties(tra_ecs PROPERTIES
        IMPORTED_LOCATION_DEBUG ${EXT_LIBS_DIR}/dll/debug/tra_ecs.dll
        IMPORTED_LOCATION_RELEASE ${EXT_LIBS_DIR}/dll/release/tra_ecs.dll
        IMPORTED_IMPLIB ${EXT_LIBS_DIR}/lib/tra_ecs.lib
    )

    add_custom_command(TARGET tra_engine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:tra_ecs>
        ${COMMON_EXPORT_LIB_DIR}/
    )
    
    add_custom_command(TARGET tra_engine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_LINKER_FILE:tra_ecs>
        ${COMMON_EXPORT_LIB_DIR}/
    )
else()
    add_library(tra_ecs SHARED IMPORTED)
    set_target_properties(tra_ecs PROPERTIES
        IMPORTED_LOCATION_DEBUG ${EXT_LIBS_DIR}/so/debug/libtra_ecs.so
        IMPORTED_LOCATION_RELEASE ${EXT_LIBS_DIR}/so/release/libtra_ecs.so
    )

    add_custom_command(TARGET tra_engine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:tra_ecs>
        ${COMMON_EXPORT_LIB_DIR}/
    )
endif()