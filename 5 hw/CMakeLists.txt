cmake_minimum_required(VERSION 3.7)

project(ridgid)

add_executable(ridgid main.cpp)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

target_link_libraries(ridgid ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )

