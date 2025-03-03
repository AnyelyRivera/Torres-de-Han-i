cmake_minimum_required(VERSION 3.31)
project(torresdehanoig4)

set(CMAKE_CXX_STANDARD 20)

# Configurar Raylib
include_directories(lib)  # Para encontrar raylib.h
link_directories(lib)     # Para encontrar libraylib.a

add_executable(TorresHanoi main.cpp)

# Enlazar Raylib y dependencias de Windows
target_link_libraries(TorresHanoi
        libraylib.a
        -lwinmm
        -lgdi32
        -lopengl32
        -lshell32
        -mwindows
)