cmake_minimum_required(VERSION 3.20)
project(Miku_Language C)

set(CMAKE_C_STANDARD 99)

# Esto le dice a CLion que busque dentro de la carpeta include de tu proyecto
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB SOURCES "src/*.c")

add_executable(Miku_Language ${SOURCES})
