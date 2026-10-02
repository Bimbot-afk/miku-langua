cmake_minimum_required(VERSION 3.20)
project(Miku_language C)

set(CMAKE_C_STANDARD 99)

# Esto le enseña al compilador a buscar en la carpeta include/
include_directories(include)

add_executable(Miku_language
        src/main.c
        src/lexer.c
)