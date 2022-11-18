cmake_minimum_required(VERSION 3.23)
project(FlyingSimulator)

find_package(Armadillo REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

set(CMAKE_CXX_STANDARD 20)

add_executable(FlyingSimulator
        src/main.cpp
        src/SolveEquations.cpp src/SolveEquations.h
        src/ChangeMetrics.cpp src/ChangeMetrics.h
        src/QuaternionLib.cpp src/QuaternionLib.h)

include_directories(
        ${ARMADILLO_INCLUDE_DIRS}
        ${OPENGL_INCLUDE_DIR}  ${GLUT_INCLUDE_DIRS}
)

target_link_libraries(FlyingSimulator
        ${ARMADILLO_LIBRARIES}
        OpenGL::GL GLUT::GLUT)