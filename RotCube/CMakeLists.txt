# CMakeList.txt : CMake project for RotCube, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Set the project name
project(RotCube)

# Find SDL2
find_package(SDL2 REQUIRED)

# Add source to this project's executable.
add_executable (RotCube "main.cpp")

# Include SDL2 libraries
set_target_properties(RotCube PROPERTIES WIN32_EXECUTABLE TRUE)
target_include_directories(RotCube PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(RotCube PRIVATE ${SDL2_LIBRARIES})

# Set properties
# TODO: Add tests and install targets if needed.
