cmake_minimum_required(VERSION 3.21)
project(majkraft C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/target)

add_subdirectory(external/glfw)

add_library(glad STATIC external/glad/src/glad.c)
target_include_directories(glad PUBLIC
        external/glad/include
)

add_subdirectory(external/cglm)

add_library(stb INTERFACE)
target_include_directories(stb INTERFACE external/stb)

file(GLOB_RECURSE SRC_FILES src/*.c src/*.h)
add_executable(majkraft ${SRC_FILES})

target_link_libraries(majkraft PRIVATE
        glad
        glfw
        cglm
        stb
)