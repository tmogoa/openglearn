cmake_minimum_required(VERSION 3.20)
project(openglearn)

set(CMAKE_CXX_STANDARD 14)

#add glad.c to executables
add_executable(openglearn main.cpp glad.c)

find_package(OpenGL REQUIRED)
#include glfw3
find_package(glfw3 3.3 REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} glfw)
