cmake_minimum_required(VERSION 3.20)

project(PokemonProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(my_app
    src/main.cpp
)

target_include_directories(my_app PRIVATE include)