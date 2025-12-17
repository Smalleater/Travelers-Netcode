cmake_minimum_required(VERSION 3.10)

project(TRA_TestEcs VERSION 0.1)

# Use C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Collect all source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Add executable target
add_executable(tra_testEcs ${SOURCES})

# Include directories
target_include_directories(tra_testEcs 
	PRIVATE 
		${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(tra_testEcs PUBLIC tra_ecs)

target_compile_definitions(tra_testEcs
    PUBLIC
        $<$<CONFIG:Debug>:_DEBUG>
        $<$<CONFIG:Release>:NDEBUG>
)

if (WIN32)
file(GLOB DLL "${COMMON_EXPORT_LIB_DIR}/*.dll")
else()
file(GLOB DLL "${COMMON_EXPORT_LIB_DIR}/*.so")
endif()

add_custom_command(TARGET tra_testEcs POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${DLL} $<TARGET_FILE_DIR:tra_testEcs>
)
