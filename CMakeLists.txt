# CMake entry point
cmake_minimum_required (VERSION 3.1)
project (deferred_shading)

find_package(OpenGL REQUIRED)

# get rid of annoying MSVC warnings.
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

# Compile external dependencies
add_subdirectory (deps)

set (CMAKE_CXX_STANDARD 11)

include_directories(
	deps/glfw-3.2/include/GLFW/
	deps/glad/include
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
)

add_executable(demo
  src/main.cpp
  deps/glad/src/glad.c
	)

target_link_libraries(demo
	${ALL_LIBS}
)
