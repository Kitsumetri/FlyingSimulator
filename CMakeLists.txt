cmake_minimum_required(VERSION 3.23)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 99)
include(FetchContent)
project(FlyingSimulator CXX C)

file(GLOB_RECURSE SOURCES_MATH src/math/*.cpp)
file(GLOB_RECURSE SOURCES_GRAPHICS src/graphics/*.cpp)
file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE SOURCES_SOIL2 src/SOIL_SRC/*.c)

find_package(Armadillo REQUIRED)
find_package(OpenGL REQUIRED)

find_package(GLUT REQUIRED)
find_package(assimp REQUIRED)

FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
)
FetchContent_MakeAvailable(glfw)
FetchContent_GetProperties(glfw)
if(NOT glfw_POPULATED)
    FetchContent_Populate(glfw)

    set(GLFW_BUILD_DOCS     off CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS    off CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES off CACHE BOOL "" FORCE)

    add_subdirectory(
            ${glfw_SOURCE_DIR}
            ${glfw_BINARY_DIR}
    )
endif()

FetchContent_Declare(
        glad
        GIT_REPOSITORY https://github.com/Dav1dde/glad.git
)
FetchContent_MakeAvailable(glad)
FetchContent_GetProperties(glad)
if(NOT glad_POPULATED)
    FetchContent_Populate(glad)
    set(
            GLAD_PROFILE "core"
            CACHE STRING "OpenGL profile"
    )
    set(
            GLAD_API "gl=4.6"
            CACHE STRING "API type/version pairs, "
            "like \"gl=3.2,gles=\", no version means latest"
    )
    set(
            GLAD_GENERATOR "c"
            CACHE STRING "Language to generate the binding for")
    add_subdirectory(
            ${glad_SOURCE_DIR}
            ${glad_BINARY_DIR}
    )
endif()

add_executable(
        FlyingSimulator
        ${SOURCES}
        ${SOURCES_MATH}
        ${SOURCES_SOIL2}
        ${SOURCES_GRAPHICS}
)

include_directories(
        ${ARMADILLO_INCLUDE_DIRS}
        ${OPENGL_INCLUDE_DIRS}
        ${GLUT_INCLUDE_DIRS}
        ${assimp_INCLUDE_DIRS}
)
target_include_directories(FlyingSimulator PUBLIC include)
target_include_directories(FlyingSimulator PUBLIC SOIL2)

target_link_libraries(FlyingSimulator
        ${ARMADILLO_LIBRARIES}
        OpenGL::GL
        GLUT::GLUT
        glfw
        glad
        ${ASSIMP_LIBRARIES}
        ${glm_LIBRARIES}
)