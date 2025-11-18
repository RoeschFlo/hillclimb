cmake_minimum_required(VERSION 3.10.0)
project(test VERSION 0.1.0 LANGUAGES C CXX)

# Raylib finden
find_package(raylib REQUIRED)

add_executable(test src/main.cpp)
##target_include_directories(test PRIVATE include)

# Raylib verlinken
target_link_libraries(test PRIVATE raylib)