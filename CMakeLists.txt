cmake_minimum_required(VERSION 3.10)
project(MyProject)

# Find SDL2 library
find_package(SDL2 REQUIRED)

# Find GLEW library
find_package(GLEW REQUIRED)
add_subdirectory(src/gl_object)
add_subdirectory(src/renderer)
# Add your source files
add_executable(my_project main.c)

# Include SDL2 headers
target_include_directories(my_project PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/gl_object)
target_include_directories(my_project PRIVATE ${SDL2_INCLUDE_DIRS})

# Include GLEW headers
target_include_directories(my_project PRIVATE ${GLEW_INCLUDE_DIRS})

# Link with SDL2, GLEW, OpenGL, and GLU libraries
target_link_libraries(my_project PRIVATE ${SDL2_LIBRARIES} ${GLEW_LIBRARIES} gl_object renderer m GL GLU)

