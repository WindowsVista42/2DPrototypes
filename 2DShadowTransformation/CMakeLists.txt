cmake_minimum_required(VERSION 3.15)
project(2DShadowTransformation)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(lib/raylib)
#find_package(raylib 3.0 REQUIRED) # Requires at least version 3.0

set(CMAKE_C_STANDARD 11) # Requires C11 standard
set(CMAKE_CXX_STANDARD 17) # Requires C11 standard

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} raylib)
