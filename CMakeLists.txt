cmake_minimum_required(VERSION 3.10)
project(CubeDemo CXX)

set(CMAKE_CXX_STANDARD 17)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

# Create executable (compile as C++ since GLM requires it)
add_executable(cube_demo main.c)

# Treat .c file as C++ for GLM compatibility
set_source_files_properties(main.c PROPERTIES LANGUAGE CXX)

# Link libraries
target_link_libraries(cube_demo
    OpenGL::OpenGL
    GLEW::GLEW
    glfw
    glm::glm
)

# Output directory
set_target_properties(cube_demo PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
